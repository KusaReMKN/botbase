# botbase
the simplest IRC client

## なにこれ

IRC ネットワークに参加するだけのクライアントプログラムです。
本当に何もせず、ただそこにいるだけです。

## つくりかた

`make` すると **pingbot** が生成されます。

```console
$ ls
LICENSE  Makefile  README.md  pingbot.c
$ make
cc -O -Wall -o pingbot pingbot.c
$ ls
LICENSE  Makefile  README.md  pingbot  pingbot.c
$ 
```

## つかいかた

少なくともホスト名（もしくは IP アドレス）を指定する必要があります。
ポート番号（もしくはサービス名）を指定することもできます。

```console
$ ./pingbot example.com
$ ./pingbot example.com 6667
$ ./pingbot 203.0.113.104
$ ./pingbot 203.0.113.104 ircd
```

終了するには <kbd><kbd>Ctrl</kbd>+<kbd>C</kbd></kbd> を押下します。

## らいせんす

BSD 2-Clause "Simplified" License
