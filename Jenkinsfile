pipeline {
    agent any

    stages {
        stage('Configure') {
            steps {
                sh 'cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
            }
        }

        stage('Build') {
            steps {
                sh 'cmake --build build --parallel'
            }
        }

        stage('Test') {
            steps {
                sh 'ctest --test-dir build --output-on-failure'
            }
        }
    }

    post {
        always {
            deleteDir()
        }
    }
}
