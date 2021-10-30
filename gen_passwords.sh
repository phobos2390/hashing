#/bin/bash

for i in {0..100} ; do 
  seed="$(date | build/tab_hash) ${i}"
  #build/password_generator -d res/english_words.txt -s "${seed}"
  build/password_generator -d res/password_list.txt -s "${seed}"
done
