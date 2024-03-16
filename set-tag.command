#! /bin/bash
git checkout stg
git merge dev
time_tag=$(date +'%H-%M-%S/%d/%m/%Y')
git tag -a $time_tag -m 'New update: $time_tag'
git push origin stg --tags
git push origin stg
git checkout dev
