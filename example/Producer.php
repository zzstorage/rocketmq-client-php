<?php
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

namespace RocketMQ;
$instanceName = "helloproducerphp";

$producer = new Producer($instanceName);
$producer->setInstanceName($instanceName);
$producer->setNamesrvAddr("tmpmq.zhuaninc.com:9876;192.168.149.15:9876");
$producer->setTcpTransportPullThreadNum(40);
$producer->getTcpTransportConnectTimeout(100);
$producer->setTcpTransportTryLockTimeout(1);
$producer->start();

//$messageQueue = new MessageQueue("topicInBrokerA", "", 1);

for ($i = 0; $i < 10; $i ++){
	$message = new Message("topicInBrokerA", "tagA");
	//$message = new Message("topicInBrokerA", "tagB", "hello world $i", "msg$i");
	//$message = new Message("topicInBrokerA", "*", "hello world $i");
	$sendResult = $producer->send($message);
	printf("|%-30s|%-40s|\n", "msgId", $sendResult->getMsgId());
	printf("|%-30s|%-40s|\n", "offsetMsgId", $sendResult->getOffsetMsgId());
	printf("|%-30s|%-40s|\n", "sendStatus", $sendResult->getSendStatus());
	printf("|%-30s|%-40s|\n", "queueId", $sendResult->getMessageQueue()->getQueueId());
	printf("|%-30s|%-40s|\n", "queueOffset", $sendResult->getQueueOffset());
    	printf("|%-30s|%-40s|\n", "body", $message->getBody());
	echo "-------------------------------------------------------------------------\n";
}

$producer->shutdown();
