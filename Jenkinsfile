pipeline {
    agent any

    stages {
        stage('Format') {
            steps {
                sh 'SKIP=clang-tidy pre-commit run --all-files --show-diff-on-failure'
            }
        }

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

        stage('Tidy') {
            steps {
                sh 'pre-commit run clang-tidy --all-files'
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
