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

include("Message.php");

$consumer = new PushConsumer("hello_groupnamezzy91");
$consumer->setInstanceName("instance");
$consumer->setNamesrvAddr("tmpmq.zhuaninc.com:9876;192.168.149.15:9876");
//$consumer->setThreadCount(1);
//$consumer->setLogPath("/home/work");
//$consumer->setListenerType(MessageListenerType::LISTENER_ORDERLY);
//$consumer->setConsumeFromWhere(ConsumeFromWhere::CONSUME_FROM_FIRST_OFFSET);
//$consumer->setMessageModel(MessageModel::BROADCASTING);
$result = array();
$count = 0;
// if thread > 1 & use echo method will core dump.
$consumer->setCallback(function ($msg) use (&$count){
    $count ++;
    echo date("Y-m-d H:i:s");
    echo " receive mesage $count, ";
    echo_msg($msg);
});
$consumer->subscribe("topicInBrokerA", "tagA");
$consumer->subscribe("helloworld", "*");
$consumer->start();
$consumer->shutdown();

