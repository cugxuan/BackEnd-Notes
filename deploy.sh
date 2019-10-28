#!/bin/bash

# 设置远程仓库的地址
remote_url=git@github.com:cugxuan/BackEnd-Notes.git
# 获取当前时间
cur_date="`date +%Y-%m-%d-%H:%M:%S`" 
# 生成_book文件
yarn build
# 删除.deploy_git/*，但是不删除.git
cd books/.vuepress
rm -rf .deploy_git/* | egrep .deploy_git/.git
if [ ! -d ".deploy_git/" ];then
	cp -R dist/ .deploy_git/
else
	cp -R dist/* .deploy_git/
fi
cd .deploy_git/
git init
git remote add origin $remote_url
git checkout -b gh-pages
git add -A
git commit -m $cur_date
git push -f origin gh-pages