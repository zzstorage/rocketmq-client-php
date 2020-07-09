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

$topicName = "topicInBrokerA";
for ($i = 0; $i < 20; $i ++){
    $message = new Message($topicName, "hello world $i");
    $message->setTags("tagA");
    //$message = new Message($topicName, "tagA", "hello world $i");
    //$message = new Message($topicName, "tagA", "msg$i", "hello world $i");
    $message->setProperty("message no", "$i");
    $message->setProperty("property", "value");
    $message->setDelayTimeLevel($i+1);
    //$properties = array(
    //    "message no" => "$i+1",
    //    "property" => "value",
    //);
    //below function will overwrite tag and keys
    //$message->setProperties($properties);
	$sendResult = $producer->send($message);
	printf("topic: %s, ",       $message->getTopic());
	printf("msgId: %s, ",       $sendResult->getMsgId());
	printf("offsetMsgId: %s, ", $sendResult->getOffsetMsgId());
	printf("sendStatus: %s, ",  $sendResult->getSendStatus());
	printf("queueId: %s, ",     $sendResult->getMessageQueue()->getQueueId());
	printf("queueOffset: %s, ", $sendResult->getQueueOffset());
    printf("body: %s\n",        $message->getBody());
    //usleep(500000);
}

$producer->shutdown();
