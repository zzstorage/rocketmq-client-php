/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "pull_consumer.h"
#include "session_credentials.h"

void PullConsumer::__construct(Php::Parameters &params){
    std::string groupName = params[0];
    this->consumer = new rocketmq::DefaultMQPullConsumer(groupName);
}

void PullConsumer::setGroup(Php::Parameters &params){
    std::string groupName = params[0];
    this->consumer->setGroupName(groupName);
}

void PullConsumer::start(){
    this->consumer->start();
}

void PullConsumer::shutdown(){
    this->consumer->shutdown();
}

Php::Value PullConsumer::getQueues(){
    Php::Array result;
    int idx = 0;
    this->consumer->fetchSubscribeMessageQueues(this->topicName, this->mqs);
    std::vector<rocketmq::MQMessageQueue>::iterator iter = mqs.begin();
    for (unsigned i = 0; i < mqs.size(); i ++ ){
        rocketmq::MQMessageQueue *mq = new rocketmq::MQMessageQueue(mqs[i]);
        result[idx++] = Php::Object(MESSAGE_QUEUE_CLASS_NAME , new MessageQueue(mq)); 
    }
    return result;
}

Php::Value PullConsumer::getNamesrvDomain(){
    return this->consumer->getNamesrvDomain();
}

void PullConsumer::setNamesrvDomain(Php::Parameters &param){
    std::string namesrv_domain = param[0];
    this->consumer->setNamesrvDomain(namesrv_domain);
}

Php::Value PullConsumer::getNamesrvAddr(){
    return this->consumer->getNamesrvAddr();
}

void PullConsumer::setNamesrvAddr(Php::Parameters &param){
    std::string namesrv_addr = param[0];
    this->consumer->setNamesrvAddr(namesrv_addr);
}

void PullConsumer::setInstanceName(Php::Parameters &param){
    std::string instanceName = param[0];
    this->consumer->setInstanceName(instanceName);
}

/*Php::Value PullConsumer::getNameSpace(){
    return this->consumer->getNameSpace();
}*/

void PullConsumer::setTopic(Php::Parameters &param){
    std::string topic = param[0];
    this->topicName= topic;
}

// pull( MessageQueue mq, string subExpression, int offset, int maxNums)
Php::Value PullConsumer::pull(Php::Parameters &param){
    Php::Value mq = param[0];
    std::string subExpression = param[1];
    int64_t offset = param[2];
    int64_t maxNums = param[3];
    MessageQueue* messageQueue = (MessageQueue*)mq.implementation();
    rocketmq::PullResult result = this->consumer->pull(messageQueue->getInstance(), subExpression, offset, maxNums);
    PullResult *pullResult = new PullResult(result);
    Php::Value pv(Php::Object(PULL_RESULT_CLASS_NAME, pullResult));
    return pv;
}

/*
   void PullConsumer::persistConsumerOffset(){
   this->consumer->persistConsumerOffset();
   }

   void PullConsumer::persistConsumerOffsetByResetOffset(){
   this->consumer->persistConsumerOffsetByResetOffset();
   }
   */

Php::Value PullConsumer::pullBlockIfNotFound(Php::Parameters &param){
    Php::Value mq = param[0];
    std::string subExpression = param[1];
    int64_t offset = param[2];
    int64_t maxNums = param[3];
    MessageQueue* messageQueue = (MessageQueue*)mq.implementation();
    rocketmq::PullResult result = this->consumer->pullBlockIfNotFound(messageQueue->getInstance(), subExpression, offset, maxNums);
    PullResult *pullResult = new PullResult(result);
    Php::Value pv(Php::Object(PULL_RESULT_CLASS_NAME, pullResult));
    return pv;
}

void PullConsumer::setSessionCredentials(Php::Parameters &param){
    std::string accessKey = param[0];
    std::string secretKey = param[1];
    std::string authChannel = param[2];

    this->consumer->setSessionCredentials(accessKey, secretKey, authChannel);
}

Php::Value PullConsumer::getSessionCredentials(){
    rocketmq::SessionCredentials sc = this->consumer->getSessionCredentials();
    SessionCredentials *sessionCredentials = new SessionCredentials(&sc);
    Php::Value pv(Php::Object(SESSION_CREDENTIALS_CLASS_NAME , sessionCredentials));
    return pv;
}


void PullConsumer::updateConsumeOffset(Php::Parameters &params){
    Php::Value mq = params[0];
    int64_t offset = params[1];

    MessageQueue* messageQueue = (MessageQueue*)mq.implementation();
    this->consumer->updateConsumeOffset(messageQueue->getInstance(), offset);
}

void PullConsumer::removeConsumeOffset(Php::Parameters &params){
    Php::Value mq = params[0];
    MessageQueue* messageQueue = (MessageQueue*)mq.implementation();
    this->consumer->removeConsumeOffset(messageQueue->getInstance());
}

Php::Value PullConsumer::fetchConsumeOffset(Php::Parameters &params){
    Php::Value mq = params[0];
    bool fromStore = params[1];
    MessageQueue* messageQueue = (MessageQueue*)mq.implementation();
    return (int64_t)this->consumer->fetchConsumeOffset(messageQueue->getInstance(), fromStore);
}


void PullConsumer::persistConsumerOffset4PullConsumer(Php::Parameters &params){
    Php::Value mq = params[0];
    MessageQueue* messageQueue = (MessageQueue*)mq.implementation();
    this->consumer->persistConsumerOffset4PullConsumer(messageQueue->getInstance());
}

void PullConsumer::setLogLevel(Php::Parameters &param){
    this->consumer->setLogLevel(rocketmq::elogLevel((int)param[0]));
}

Php::Value PullConsumer::getLogLevel(){
    return this->consumer->getLogLevel();
}

/*void PullConsumer::setLogPath(Php::Parameters &param){
    this->consumer->setLogPath(param[0]);
}*/

void PullConsumer::setLogFileSizeAndNum(Php::Parameters &param){
    this->consumer->setLogFileSizeAndNum(param[0], param[1]);
}

void registerPullConsumer(Php::Namespace &rocketMQNamespace){
    Php::Class<PullConsumer> pullConsumer("PullConsumer");
    pullConsumer.method<&PullConsumer::__construct>("__construct", { Php::ByVal("groupName", Php::Type::String), });
    pullConsumer.method<&PullConsumer::setInstanceName>("setInstanceName", { Php::ByVal("instance", Php::Type::String), });
    pullConsumer.method<&PullConsumer::setTopic>("setTopic", { Php::ByVal("topic", Php::Type::String), });
    pullConsumer.method<&PullConsumer::start>("start");
    pullConsumer.method<&PullConsumer::shutdown>("shutdown");
    pullConsumer.method<&PullConsumer::getQueues>("getQueues");

    pullConsumer.method<&PullConsumer::setNamesrvAddr>("setNamesrvAddr", { Php::ByVal("namesrvAddr", Php::Type::String), });
    pullConsumer.method<&PullConsumer::getNamesrvAddr>("getNamesrvAddr");

    pullConsumer.method<&PullConsumer::setNamesrvDomain>("setNamesrvDomain", { Php::ByVal("nameserver", Php::Type::String), });
    pullConsumer.method<&PullConsumer::getNamesrvDomain>("getNamesrvDomain");

    pullConsumer.method<&PullConsumer::setGroup>("setGroup", { Php::ByVal("group", Php::Type::String), });
    pullConsumer.method<&PullConsumer::pull>("pull", {
            Php::ByVal("mq", MESSAGE_QUEUE_CLASS_NAME),
            Php::ByVal("subExpression", Php::Type::String),
            Php::ByVal("offset", Php::Type::Numeric),
            Php::ByVal("maxNums", Php::Type::Numeric),
            });

    pullConsumer.method<&PullConsumer::pullBlockIfNotFound>("pullBlockIfNotFound", {
            Php::ByVal("mq", MESSAGE_QUEUE_CLASS_NAME),
            Php::ByVal("subExpression", Php::Type::String),
            Php::ByVal("offset", Php::Type::Numeric),
            Php::ByVal("maxNums", Php::Type::Numeric),
            });

    pullConsumer.method<&PullConsumer::setSessionCredentials>("setSessionCredentials", {
            Php::ByVal("accessKey", Php::Type::String),
            Php::ByVal("secretKey", Php::Type::String),
            Php::ByVal("authChannel", Php::Type::String),
            });
    pullConsumer.method<&PullConsumer::getSessionCredentials>("getSessionCredentials");
    pullConsumer.method<&PullConsumer::updateConsumeOffset>("updateConsumeOffset", {
            Php::ByVal("mq", MESSAGE_QUEUE_CLASS_NAME),
            Php::ByVal("offset", Php::Type::Numeric),
            });
    pullConsumer.method<&PullConsumer::persistConsumerOffset4PullConsumer>("persistConsumerOffset4PullConsumer", {
            Php::ByVal("mq", MESSAGE_QUEUE_CLASS_NAME),
            });
    pullConsumer.method<&PullConsumer::removeConsumeOffset>("removeConsumeOffset", { Php::ByVal("mq", MESSAGE_QUEUE_CLASS_NAME), });
    pullConsumer.method<&PullConsumer::fetchConsumeOffset>("fetchConsumeOffset", {
            Php::ByVal("mq", MESSAGE_QUEUE_CLASS_NAME),
            Php::ByVal("fromStore", Php::Type::Bool),
            });

    pullConsumer.method<&PullConsumer::setLogLevel>("setLogLevel", {Php::ByVal("inputLevel", Php::Type::Numeric),});
    pullConsumer.method<&PullConsumer::getLogLevel>("getLogLevel");
    pullConsumer.method<&PullConsumer::setLogFileSizeAndNum>("setLogFileSizeAndNum", {Php::ByVal("fileNum", Php::Type::Numeric),Php::ByVal("perFileSize", Php::Type::Numeric),});
   //pullConsumer.method<&PullConsumer::setLogPath>("setLogPath", {Php::ByVal("logPath", Php::Type::String),});

   /*pullConsumer.method<&PullConsumer::setNameSpace>("setNameSpace", {Php::ByVal("nameSpace", Php::Type::String),});
   pullConsumer.method<&PullConsumer::getNameSpace>("getNameSpace");

   pullConsumer.method<&PullConsumer::getGroupName>("getGroupName");
   pullConsumer.method<&PullConsumer::setGroupName>("setGroupName", {Php::ByVal("groupname", Php::Type::String),});

   pullConsumer.method<&PullConsumer::getInstanceName>("getInstanceName");

   pullConsumer.method<&PullConsumer::version>("version");*/

    rocketMQNamespace.add(pullConsumer);
}
