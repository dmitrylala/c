# shell

Для работы с шеллом сначала необходимо выполнить программу main, затем печатается строка-приглашение, после чего можно вводить команды, как в обычном шелле. Поддержка следующих операндов: <, >, ||, |, &&, &, ; (скобки у меня нормально так и не вышло сделать). Тестовые команды:

ps -a | grep bin

ls -al /bin | wc -l

ls -al | grep .c | wc -l

ls -lR > file1

grep include polis.h

grep include < polis.h

grep include < polis.h > file2

ls; grep include polis.h

ls; pwd; grep include polis.h

ps -a | grep bin; ls -al | grep .c | wc -l

ls -al | grep .c; grep include polis.h

grep include < polis.h > file2; ls

pwd && who

pwd || who

ls; pwd; grop include polis.h && echo 20

ls; pwd; grep include polis.h && echo 20

grep include polis.h || ls && pwd

ls && ls && ls

ls || grep include polis.h || who

pwd && ls | tail && ls

pwd || ls | tail || grep include polis.h
