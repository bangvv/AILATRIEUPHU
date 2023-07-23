extern "C"
{
  #include "../headers/Utils.h"
}

extern "C" int sockfd;
#include "../headers/BackEnd.h"
#include "CountTime.h"
#include <QDebug>

std::string BackEnd::server_ip = "127.0.0.1";
int BackEnd::server_port = 5500;

void *thread_recv(void *arg)
{
  pthread_detach(pthread_self());

  Message msg;
  int recvBytes;
  int incorrect_answer[2];
  int i = 0;
  int numQuestion = 0;

  while (1)
  {
    recvBytes = recv(sockfd, &msg, sizeof(msg), 0);
    if (recvBytes < 0)
    {
      perror("The server terminated prematurely\n");
      exit(0);
      return NULL;
    }
    else
    {
      switch (msg.type)
      {
      case CHANGE_QUESTION:
      case QUESTION:
        BackEnd::instance->setIsRunGame(true);
        numQuestion = atoi(strtok(msg.value, "|"));
        BackEnd::instance->question = strtok(NULL, "|");
        BackEnd::instance->a = strtok(NULL, "|");
        BackEnd::instance->b = strtok(NULL, "|");
        BackEnd::instance->c = strtok(NULL, "|");
        BackEnd::instance->d = strtok(NULL, "|");

        if ((i == 0) || (BackEnd::instance->prize + 1 == numQuestion)){
          emit BackEnd::instance->questionChanged();
          emit BackEnd::instance->aChanged();
          emit BackEnd::instance->bChanged();
          emit BackEnd::instance->cChanged();
          emit BackEnd::instance->dChanged();
          i++;
        }
        break;
      case CORRECT_ANSWER:
        BackEnd::instance->correct_answer = atoi(strtok(msg.value, " "));
        BackEnd::instance->reward = atoi(strtok(NULL, " "));
        emit BackEnd::instance->rewardChanged();
        emit BackEnd::instance->correctAnswer();
        break;
      case WIN:
        BackEnd::instance->setIsRunGame(false);
        BackEnd::instance->status_game = WIN;
        BackEnd::instance->correct_answer = atoi(strtok(msg.value, " "));
        BackEnd::instance->reward = atoi(strtok(NULL, " "));
        emit BackEnd::instance->rewardChanged();
        BackEnd::instance->prize++;
        emit BackEnd::instance->correctAnswer();
        emit BackEnd::instance->statusGameChanged();
        return NULL;
      case WIN_PVP:
        BackEnd::instance->setIsRunGame(false);
        BackEnd::instance->status_game = WIN;

        if(strlen(msg.value) > 0){
          BackEnd::instance->correct_answer = atoi(msg.value);
          emit BackEnd::instance->lose();
        } else {
          emit BackEnd::instance->showResultPvP();
        }
        return NULL;
      case LOSE:
        BackEnd::instance->setIsRunGame(false);
        BackEnd::instance->status_game = LOSE;
        emit BackEnd::instance->statusGameChanged();
        BackEnd::instance->correct_answer = atoi(msg.value);
        emit BackEnd::instance->lose();
        return NULL;
      case LOSE_PVP:
        BackEnd::instance->setIsRunGame(false);
        BackEnd::instance->status_game = LOSE;
        emit BackEnd::instance->showResultPvP();
        return NULL;
      case STOP_GAME_PVP:
      case OVER_TIME_PVP:
          //Todo:
          break;
      case STOP_GAME:
      case OVER_TIME:
        BackEnd::instance->setIsRunGame(false);
        BackEnd::instance->status_game = LOSE;
        emit BackEnd::instance->statusGameChanged();
        return NULL;
      case FIFTY_FIFTY:
        incorrect_answer[0] = atoi(strtok(msg.value, " "));
        incorrect_answer[1] = atoi(strtok(NULL, " "));
        switch (incorrect_answer[0])
        {
        case 1:
          BackEnd::instance->a = "";
          emit BackEnd::instance->aChanged();
          break;
        case 2:
          BackEnd::instance->b = "";
          emit BackEnd::instance->bChanged();
          break;
        case 3:
          BackEnd::instance->c = "";
          emit BackEnd::instance->cChanged();
          break;
        case 4:
          BackEnd::instance->d = "";
          emit BackEnd::instance->dChanged();
          break;
        }

        switch (incorrect_answer[1])
        {
        case 1:
          BackEnd::instance->a = "";
          emit BackEnd::instance->aChanged();
          break;
        case 2:
          BackEnd::instance->b = "";
          emit BackEnd::instance->bChanged();
          break;
        case 3:
          BackEnd::instance->c = "";
          emit BackEnd::instance->cChanged();
          break;
        case 4:
          BackEnd::instance->d = "";
          emit BackEnd::instance->dChanged();
          break;
        }
        break;
      case CALL_PHONE:
        if (msg.value[0] == '1')
        {
          BackEnd::instance->call_phone_answer = "A";
        }
        else if (msg.value[0] == '2')
        {
          BackEnd::instance->call_phone_answer = "B";
        }
        else if (msg.value[0] == '3')
        {
          BackEnd::instance->call_phone_answer = "C";
        }
        else if (msg.value[0] == '4')
        {
          BackEnd::instance->call_phone_answer = "D";
        }
        emit BackEnd::instance->callPhoneAnswerChanged();
        emit BackEnd::instance->openCallPhone();
        break;
      case VOTE:
        BackEnd::instance->voteA = atoi(strtok(msg.value, " "));
        BackEnd::instance->voteB = atoi(strtok(NULL, " "));
        BackEnd::instance->voteC = atoi(strtok(NULL, " "));
        BackEnd::instance->voteD = atoi(strtok(NULL, " "));

        emit BackEnd::instance->voteAChanged();
        emit BackEnd::instance->voteBChanged();
        emit BackEnd::instance->voteCChanged();
        emit BackEnd::instance->voteDChanged();

        emit BackEnd::instance->openVote();
        break;
      case FOUND_PLAYER:
        BackEnd::instance->setEnemyName(msg.value);
        emit BackEnd::instance->enemyNameChanged();
        emit BackEnd::instance->foundPlayer();
        break;
      case NOT_FOUND_PLAYER:
        emit BackEnd::instance->notFoundPlayer();
        break;
      case ENEMY_CURRENT_DATA:
        BackEnd::instance->enemy_current_question = atoi(strtok(msg.value, " "));
        BackEnd::instance->enemy_seconds = atoi(strtok(NULL, " "));
        BackEnd::instance->enemy_status = strtok(NULL, " ");

        emit BackEnd::instance->enemyCurrentQuestionChanged();
        emit BackEnd::instance->enemySecondsChanged();
        emit BackEnd::instance->enemyStatusChanged();
        break;
      case RECV_SCORE_ALONE:
        qDebug() << "Receiver data from server";
 //       BackEnd::instance->resetScoreScreen();
        BackEnd::instance->reciScoreList(msg.value);
        msg.type = msg_type::NONE;
        break;
      default:
        break;
      }
    }
  }

  pthread_exit(NULL);
}

BackEnd *BackEnd::instance = nullptr;

BackEnd::BackEnd(QObject *parent) : QObject(parent)
  , m_isRunGame{false}
{
  user_name = "";
  prize = 0;
  reward = 0;
  question = "";
  a = "";
  b = "";
  c = "";
  d = "";

  if (instance == nullptr)
  {
    instance = this;
  }
  QObject::connect(this,&BackEnd::startCountTime,&CountTime::getCountTime(),&CountTime::Start);
  QObject::connect(this,&BackEnd::stopCountTime,&CountTime::getCountTime(),&CountTime::Stop);
  QObject::connect(this,&BackEnd::statusGameChanged,this,&BackEnd::saveResult);
  QObject::connect(this,&BackEnd::signalUpdateScoreList,this,&BackEnd::Update);
  m_ScoresList.clear();
}

BackEnd::~BackEnd()
{
  delete instance;
}
void BackEnd::countTimer()
{
}

QString BackEnd::getUserName() const
{
  return user_name;
}

int BackEnd::getPrize()
{
  return prize;
}

void BackEnd::setPrize(int value)
{
  prize = value;
}

QString BackEnd::getQuestion()
{
  return question;
}

QString BackEnd::getA()
{
  return a;
}

QString BackEnd::getB()
{
  return b;
}

QString BackEnd::getC()
{
  return c;
}

QString BackEnd::getD()
{
  return d;
}

QString BackEnd::getCallPhoneAnswer()
{
  return call_phone_answer;
}

int BackEnd::getCorrectAnswer()
{
  return correct_answer;
}

int BackEnd::getReward()
{
  return reward;
}

void BackEnd::setReward(int value)
{
  reward = value;
}

int BackEnd::getVoteA()
{
  return voteA;
}

int BackEnd::getVoteB()
{
  return voteB;
}

int BackEnd::getVoteC()
{
  return voteC;
}

int BackEnd::getVoteD()
{
  return voteD;
}

void BackEnd::setUserName(const QString &value)
{
  user_name = value;
}

QString BackEnd::getEnemyName() const
{
  return enemy_name;
}

void BackEnd::setEnemyName(const QString &value)
{
  enemy_name = value;
}

int BackEnd::getEnemyCurrentQuestion()
{
  return enemy_current_question;
}

void BackEnd::setEnemyCurrentQuestion(int value)
{
  enemy_current_question = value;
}

int BackEnd::getEnemySeconds()
{
  return enemy_seconds;
}

void BackEnd::setEnemySeconds(int value)
{
  enemy_seconds = value;
}

QString BackEnd::getEnemyStatus()
{
  return enemy_status;
}

void BackEnd::setEnemyStatus(QString value)
{
  enemy_status = value;
}

int BackEnd::getStatusGame()
{
  return status_game;
}

void BackEnd::setStatusGame(int value)
{
  status_game = value;
}

void BackEnd::connectToServer()
{
  char ip[16];
  strcpy(ip, this->server_ip.c_str());
  if (connect_to_server(ip, this->server_port))
  {
    emit connectSuccess();
  }
  else
  {
    emit connectFail();
  }
  // printf("%s %d\n", BackEnd::server_ip, BackEnd::server_port);
}

void BackEnd::disconnectToServer()
{
  disconnect_to_server();
}

void BackEnd::signIn(QString username, QString password)
{
  char user[100];
  char pass[100];
  strcpy(user, username.toStdString().c_str());
  strcpy(pass, password.toStdString().c_str());
  int loginStatus = login(user, pass);
  if (loginStatus == LOGIN_SUCCESS)
  {
    user_name = username;
    emit loginSuccess();
  }
  else if (loginStatus == LOGGED_IN)
  {
    emit loggedIn();
  }
  else if (loginStatus == ACCOUNT_BLOCKED)
  {
    emit accountBlocked();
  }
  else if (loginStatus == ACCOUNT_NOT_EXIST)
  {
    emit accountNotExist();
  }
  else if (loginStatus == WRONG_PASSWORD)
  {
    emit wrongPassword();
  }
}

void BackEnd::signUp(QString username, QString password)
{
  char user_name[100];
  char pass_word[100];
  strcpy(user_name, username.toStdString().c_str());
  strcpy(pass_word, password.toStdString().c_str());
  int registerStatus = signup(user_name, pass_word);
  if (registerStatus == SIGNUP_SUCCESS)
  {
    emit signupSuccess();
  }
  else if (registerStatus == ACCOUNT_EXIST)
  {
    emit accountExist();
  }
}

void BackEnd::logOut()
{
  logout();
}

void BackEnd::changePassword(QString newPassword)
{
  char password[100];
  strcpy(password, newPassword.toLocal8Bit().constData());
  int re = change_password(password);
  if (re == CHANGE_PASSWORD_SUCCESS)
  {
    emit changePasswordSuccess();
  }
  else if (re == SAME_OLD_PASSWORD)
  {
    emit sameOldPassword();
  }
}

void BackEnd::playAlone()
{
  Message msg;
  int sendBytes;

  msg.type = PLAY_ALONE;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }

  pthread_t tid;
  pthread_create(&tid, NULL, thread_recv, NULL);
}

void BackEnd::playPvP()
{
  Message msg;
  int sendBytes;

  msg.type = PLAY_PVP;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }

  pthread_t tid;
  pthread_create(&tid, NULL, thread_recv, NULL);
}

void BackEnd::requestResultList(bool isScoreAlone)
{
    Message msg;
    int sendBytes;

    qDebug() << "requestResultList : isScoreAlone: " << isScoreAlone;
    if (isScoreAlone)
        msg.type = RECV_SCORE_ALONE;
    else
    {}

    sendBytes = send(sockfd, &msg, sizeof(msg), 0);
    if (sendBytes < 0)
    {
      perror("The server terminated prematurely");
      exit(0);
      return;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, thread_recv, NULL);
}

void BackEnd::choiceAnswer(int answer, int thoi_gian_tra_loi)
{
  Message msg;
  int sendBytes;
  char str[100];

  msg.type = CHOICE_ANSWER;
  sprintf(str, "%d|%d", answer, thoi_gian_tra_loi);
  strcpy(msg.value, str);
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::fiftyFifty()
{
  Message msg;
  int sendBytes;

  msg.type = FIFTY_FIFTY;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::callPhone()
{
  Message msg;
  int sendBytes;

  msg.type = CALL_PHONE;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::vote()
{
  Message msg;
  int sendBytes;

  msg.type = VOTE;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::changeQuestion()
{
  Message msg;
  int sendBytes;

  msg.type = CHANGE_QUESTION;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::stopGame()
{
  Message msg;
  int sendBytes;

  msg.type = STOP_GAME;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::overTime()
{
  Message msg;
  int sendBytes;

  msg.type = OVER_TIME;
  sendBytes = send(sockfd, &msg, sizeof(msg), 0);
  if (sendBytes < 0)
  {
    perror("The server terminated prematurely");
    exit(0);
    return;
  }
}

void BackEnd::setIsRunGame(bool isRunGame)
{
    if (isRunGame)
        emit startCountTime();
    else
         emit stopCountTime();
    m_isRunGame = isRunGame;
}

bool BackEnd::isRunGame() const
{
    return m_isRunGame;
}

void BackEnd::resetCountSeconds()
{
    m_isRunGame = false;
}
int BackEnd::m_SizeScoresList = 0;
void BackEnd::saveResult()
{
    qDebug() << "sai";
    if(m_ScoresList.size()!= 0)
        m_SizeScoresList= m_ScoresList.size();
    else
        m_SizeScoresList++;

    resetCountSeconds();

    Message msg;
    int sendBytes;
    QString vls = user_name+'|'+QString::number(reward)+'|'+QString::number(CountTime::getCountTime().getSecond());
    msg.type = SEND_SCORE_ALONE;

    memset(msg.value,'\0',1024);

    QByteArray utf8Data = vls.toUtf8();
    memcpy(msg.value, utf8Data.data(), utf8Data.size());
    qDebug() << "send value xuong server: " << msg.value << "sizeof: " << sizeof(msg.value);

    sendBytes = send(sockfd, &msg, sizeof(msg), 0);
    if (sendBytes < 0)
    {
      perror("The server terminated prematurely");
      exit(0);
      return;
    }
}

void BackEnd::Update()
{
    Scores::instance()->updateData(false, m_ScoresList);
}

QVector<ScorePointer> BackEnd::getScoresList() const
{
    return m_ScoresList;
}
std::mutex BackEnd::mtx;
void BackEnd::reciScoreList(const QString& data)
{
//    std::lock_guard<std::mutex> lock(mtx);
    qDebug() << "OKOK";
    qDebug() << "bang check:";
    QString str = data;
    QStringList list = str.split('|');
    qDebug() << "size list: " << list.size();
    //if (list.size() == 4)
    {
        // qDebug() << list.at(0) << ' : ' << list.at(1) << ' : ' << list.at(2) << ' : ' <<list.at(3);
        ScorePointer data{list.at(0),list.at(1),list.at(2)+'$',QString::number(stoi(list.at(3).toStdString())/60)+'m'+QString::number(stoi(list.at(3).toStdString())%60)+'s'};
        qDebug() << "m_ScoresList.empty(): " << m_ScoresList.empty();
        qDebug() << "m_ScoresList.size(): " << m_ScoresList.size();
        qDebug() << "!m_ScoresList.contains(data): " << !m_ScoresList.contains(data);

        if(m_ScoresList.empty()||!m_ScoresList.contains(data)){
            if (m_ScoresList.size()<10){
                m_ScoresList.append({QString::number(m_ScoresList.size()+1),list.at(1),list.at(2)+'$',QString::number(stoi(list.at(3).toStdString())/60)+'m'+QString::number(stoi(list.at(3).toStdString())%60)+'s'});
                emit signalUpdateScoreList();
            }else{
                m_ScoresList.erase(m_ScoresList.begin());
                for(auto i = m_ScoresList.begin(); i!= m_ScoresList.end(); i++)
                {
                    i->m_stt = QString::number(stoi((i->m_stt).toStdString())-1);
                }
                m_ScoresList.append({QString::number(m_ScoresList.size()+1),list.at(1),list.at(2)+'$',QString::number(stoi(list.at(3).toStdString())/60)+'m'+QString::number(stoi(list.at(3).toStdString())%60)+'s'});
                emit signalUpdateScoreList();
            }           
        }
    }
}

//void BackEnd::resetScoreScreen() {
//    qDebug() << "Reset OK";
//  Scores::instance()->updateData(true, m_ScoresList);
//}
