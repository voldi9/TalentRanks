#!/bin/bash
git add README.md gitupdate.sh
cd app
git add *.cpp *.hpp *.sql *.sh
cd ../copyranks
git add *.cpp *.hpp *.sql *.sh

git commit
git push
