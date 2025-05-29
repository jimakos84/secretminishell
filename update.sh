#!/bin/bash

git checkout main
git pull origin main
git checkout fix_lst
git merge main
git push -u origin fix_lst
