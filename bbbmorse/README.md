# bbbmorse

## About
`bbbmorse` read text files and flashes content on beaglebone black's usr leds.

## Prerequisites 
* ARM GNU toolchain

Install the `arm-linux-gnueabi` toolchain.

Ubuntu users may type into terminal:

`$ sudo apt-get install arm-linux-gnueabi`

## Installation
Open a Terminal on Host or Remote device
```
$ git clone https://github.com/jasonleethomas/beaglebone
$ cd ./beaglebone/bbbmorse
$ make
```
Transfer binary file to beaglebone black

`$ sudo scp bbbmorse 192.168.7.2:/home`

## Usage
bbbmorse [filename] [charlimit] [ledindex]
```
$ bbbmorse /home/beaglebone/bbbmorse/message.txt 1024 0
$ bbbmorse /home/beaglebone/bbbmorse/message.txt 1024
$ bbbmorse /home/beaglebone/bbbmorse/message.txt 0
$ bbbmorse /home/beaglebone/bbbmorse/message.txt
```
### Options
* filename 	
...absolute filepath of textfile containing message. if not specified, programs looks for ./message.txt, then exits if not found.
* charlimit	
...character limit of message, program will flash less than or equal to this number. if not specified, program assumes sets limit to 512
* ledindex 
...chooses beaglebone's led, usr(0-3). if not specified, program uses usr 0.
		

