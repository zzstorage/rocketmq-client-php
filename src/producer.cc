#include "producer.h"
#include "message_queue.h"
#include "session_credentials.h"
#include "send_result.h"

void Producer::__construct(Php::Parameters &param){
    std::string groupName = param[0];
    this->producer = new rocketmq::DefaultMQProducer(groupName);
}

void Producer::setInstanceName(Php::Parameters &param){
    std::string instanceName = param[0];
    this->producer->setInstanceName(instanceName);
}

Php::Value Producer::getInstanceName(){
    return this->producer->getInstanceName();
}


void Producer::setGroupName(Php::Parameters &param){
    std::string groupName = param[0];
    this->producer->setGroupName(groupName);
}

Php::Value Producer::getGroupName(){
    return this->producer->getGroupName();
}

void Producer::setNamesrvAddr(Php::Parameters &param){
    std::string nameserver =  param[0];
    this->producer->setNamesrvAddr(nameserver);
}

void Producer::start(){
    this->producer->start();
}

//  @see SendResult send(MQMessage& msg, const MQMessageQueue& mq);
Php::Value Producer::send(Php::Parameters &params){
    Php::Value pvMessage = params[0];
    Message *message = (Message *)pvMessage.implementation();
    if (params.size() == 1){
        rocketmq::SendResult sr = this->producer->send(message->getMQMessage());
        Php::Value pv(Php::Object(SEND_RESULT_CLASS_NAME, new SendResult(sr)));
        return pv;
    }

    Php::Value pvMessageQueue = params[1];
    MessageQueue* messageQueue = (MessageQueue*)pvMessageQueue.implementation();
    rocketmq::SendResult sr = this->producer->send(message->getMQMessage(), messageQueue->getInstance());
    Php::Value pv(Php::Object(SEND_RESULT_CLASS_NAME, new SendResult(sr)));
    return pv;
}

Php::Value Producer::getMQClientId(){
    return this->producer->getMQClientId();
}

Php::Value Producer::getNamesrvAddr(){
    return this->producer->getNamesrvAddr();
}

Php::Value Producer::getTopicMessageQueueInfo(Php::Parameters &params){
    std::string topic = params[0];
    Php::Array result;

    std::vector<rocketmq::MQMessageQueue> mqs = this->producer->getTopicMessageQueueInfo(topic);
    std::vector<rocketmq::MQMessageQueue>::iterator iter = mqs.begin();
    int idx = 0;

    for (; iter != mqs.end(); ++iter) {
        rocketmq::MQMessageQueue mq = (*iter);
        result[idx++] = Php::Object(MESSAGE_QUEUE_CLASS_NAME , new MessageQueue(mq)); 
    }

    return result;
}

void Producer::setSessionCredentials(Php::Parameters &param){
    std::string accessKey = param[0];
    std::string secretKey = param[1];
    std::string authChannel = param[2];

    this->producer->setSessionCredentials(accessKey, secretKey, authChannel);
}

Php::Value Producer::getSessionCredentials(){
    rocketmq::SessionCredentials sc = this->producer->getSessionCredentials();
    SessionCredentials *sessionCredentials = new SessionCredentials(&sc);
    Php::Value pv(Php::Object(SESSION_CREDENTIALS_CLASS_NAME , sessionCredentials));
    return pv;
}

void Producer::setNamesrvDomain(Php::Parameters &param){
    std::string domain = param[0];
    return this->producer->setNamesrvDomain(domain);
}

// int getRetryTimes() const;
Php::Value Producer::getRetryTimes(){
    return this->producer->getRetryTimes();
}

// void setRetryTimes(int times);
void Producer::setRetryTimes(Php::Parameters &param){
    this->producer->setRetryTimes(param[0]);
}

        // void setTcpTransportPullThreadNum(int num);
void Producer::setTcpTransportPullThreadNum(Php::Parameters &param){
    this->producer->setTcpTransportPullThreadNum((int64_t)param[0]);
}

// const int getTcpTransportPullThreadNum() const;
Php::Value Producer::getTcpTransportPullThreadNum(){
    return (int64_t)this->producer->getTcpTransportPullThreadNum();
}

// void setTcpTransportConnectTimeout(uint64_t timeout);  // ms
void Producer::setTcpTransportConnectTimeout(Php::Parameters &param){
    this->producer->setTcpTransportConnectTimeout((int64_t)param[0]);
}
// const uint64_t getTcpTransportConnectTimeout() const;
Php::Value Producer::getTcpTransportConnectTimeout(){
    return (int64_t)this->producer->getTcpTransportConnectTimeout();
}

// void setTcpTransportTryLockTimeout(uint64_t timeout);  // ms
void Producer::setTcpTransportTryLockTimeout(Php::Parameters &param){
    this->producer->setTcpTransportTryLockTimeout((int64_t)param[0]);
}

// const uint64_t getTcpTransportConnectTimeout() const;
Php::Value Producer::getTcpTransportTryLockTimeout(){
    return (int64_t)this->producer->getTcpTransportTryLockTimeout();
}

//void setUnitName(std::string unitName);
void Producer::setUnitName(Php::Parameters &param){
    this->producer->setUnitName(param[0]);
}
//const std::string& getUnitName();
Php::Value Producer::getUnitName(){
    return this->producer->getUnitName();
}


void registerProducer(Php::Namespace &rocketMQNamespace){
    Php::Class<Producer> producerClass("Producer");
    producerClass.method<&Producer::getMQClientId>("getMQClientId");
    producerClass.method<&Producer::__construct>("__construct", { 
            Php::ByVal("groupName", Php::Type::String), 
            });

    producerClass.method<&Producer::getInstanceName>("getInstanceName");
    producerClass.method<&Producer::setInstanceName>("setInstanceName", { 
            Php::ByVal("groupName", Php::Type::String), 
            });

    producerClass.method<&Producer::getNamesrvAddr>("getNamesrvAddr");
    producerClass.method<&Producer::setNamesrvAddr>("setNamesrvAddr", { 
            Php::ByVal("nameserver", Php::Type::String), 
            });
    producerClass.method<&Producer::setNamesrvDomain>("setNamesrvDomain", {
            Php::ByVal("domain", Php::Type::String),
            });

    producerClass.method<&Producer::getGroupName>("getGroupName");
    producerClass.method<&Producer::setGroupName>("setGroupName", {
            Php::ByVal("groupName", Php::Type::String),
            });

    producerClass.method<&Producer::send>("send", {
            Php::ByVal("message", MESSAGE_CLASS_NAME),
            });
    producerClass.method<&Producer::getSessionCredentials>("getSessionCredentials");
    producerClass.method<&Producer::setSessionCredentials>("setSessionCredentials", {
            Php::ByVal("accessKey", Php::Type::String),
            Php::ByVal("secretKey", Php::Type::String),
            Php::ByVal("authChannel", Php::Type::String),
            });

    producerClass.method<&Producer::getTopicMessageQueueInfo>("getTopicMessageQueueInfo", {
            Php::ByVal("topic", Php::Type::String),
            });

    producerClass.method<&Producer::start>("start");

    producerClass.method<&Producer::setRetryTimes>("setRetryTimes", { Php::ByVal("retryTimes", Php::Type::Numeric), });
    producerClass.method<&Producer::getRetryTimes>("getRetryTimes");

    producerClass.method<&Producer::getTcpTransportTryLockTimeout>("getTcpTransportTryLockTimeout");
    producerClass.method<&Producer::setTcpTransportTryLockTimeout>("setTcpTransportTryLockTimeout",{ Php::ByVal("timeout", Php::Type::Numeric), });
    producerClass.method<&Producer::getTcpTransportConnectTimeout>("getTcpTransportConnectTimeout");
    producerClass.method<&Producer::setTcpTransportConnectTimeout>("setTcpTransportConnectTimeout", {Php::ByVal("timeout", Php::Type::Numeric), });
    producerClass.method<&Producer::getTcpTransportPullThreadNum>("getTcpTransportPullThreadNum", {Php::ByVal("threadNum", Php::Type::Numeric), });
    producerClass.method<&Producer::setTcpTransportPullThreadNum>("setTcpTransportPullThreadNum", {Php::ByVal("threadNum", Php::Type::Numeric), });
    producerClass.method<&Producer::getUnitName>("getUnitName");
    producerClass.method<&Producer::setUnitName>("setUnitName", {Php::ByVal("unitName", Php::Type::String),});

    rocketMQNamespace.add(producerClass);
}


