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

#include "send_result.h"

SendResult::SendResult(rocketmq::SendResult& sendResult){
	this->sendResult = sendResult;
}
Php::Value SendResult::getMsgId(){
	return this->sendResult.getMsgId();
}
Php::Value SendResult::getOffsetMsgId(){
	return this->sendResult.getOffsetMsgId();
}
Php::Value SendResult::getSendStatus(){
	return (int64_t)this->sendResult.getSendStatus();
}

Php::Value SendResult::getMessageQueue(){
	return Php::Object(MESSAGE_QUEUE_CLASS_NAME, new MessageQueue(this->sendResult.getMessageQueue()));
}

Php::Value SendResult::getQueueOffset(){
	return (int64_t) this->sendResult.getQueueOffset();
}

Php::Value SendResult::getRegionId(){
	return this->sendResult.getRegionId();
}

Php::Value SendResult::getTransactionId(){
	std::string transactionId = this->sendResult.getTransactionId();
	return transactionId;
}

void SendResult::setRegionId(Php::Parameters &param){
	std::string regionId = param[0];
	this->sendResult.setRegionId(regionId);
}

void registerSendResult(Php::Namespace &rocketMQNamespace){
	Php::Class<SendResult> sendResultClass("SendResult");

	sendResultClass.method<&SendResult::getMsgId>("getMsgId");
	sendResultClass.method<&SendResult::getOffsetMsgId>("getOffsetMsgId");
	sendResultClass.method<&SendResult::getSendStatus>("getSendStatus");
	sendResultClass.method<&SendResult::getMessageQueue>("getMessageQueue");
	sendResultClass.method<&SendResult::getQueueOffset>("getQueueOffset");
	sendResultClass.method<&SendResult::getTransactionId>("getTransactionId");
	sendResultClass.method<&SendResult::getRegionId>("getRegionId");
	sendResultClass.method<&SendResult::setRegionId>("setRegionId", {Php::ByVal("regionId", Php::Type::String),});

	rocketMQNamespace.add(sendResultClass);
}

