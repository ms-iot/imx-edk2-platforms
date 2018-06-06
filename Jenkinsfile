// Copyright (c) Microsoft Corporation. All rights reserved.

node {
   stage('Clean workspace') {
       deleteDir()
   }

   stage('Setup Docker') {
      checkout scm
      def environment  = docker.build 'edk2-baseimage'

      environment.inside {
         stage('Fetch code') {
            dir ('edk2') {
               checkout([
                  $class: 'GitSCM',
                  branches: [[name: '*/master']],
                  doGenerateSubmoduleConfigurations: false,
                  extensions: [[$class: 'SubmoduleOption',
                     disableSubmodules: false,
                     parentCredentials: false,
                     recursiveSubmodules: false,
                     reference: '',
                     trackingSubmodules: false
                  ]],
                  submoduleCfg: [],
                  userRemoteConfigs: [[url: 'git@github.com:tianocore/edk2.git']]
               ])
            }
            dir ('edk2-platforms') {
               checkout scm
            }
         }
         stage('Build') {
            sh "#!/bin/bash \n" + '/home/jenkins/edk2-build.sh'
         }
      }
   }
}