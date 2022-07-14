# exam_rank_06
Last exam of the 42 Cursus. Here to help further student in their struggle !

<p align="center">
	<img src=".sources/grade.png" alt="Exam Grade" width="280" height="300"/>
</p>

<p align="center">
	<img src="https://img.shields.io/github/languages/top/Spidfail/exam_rank_06" alt="Most used language"/>
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Spidfail/exam_rank_06">
	<img alt="Repo size" src="https://img.shields.io/github/repo-size/Spidfail/exam_rank_06 ">
	<img src="https://img.shields.io/github/license/Spidfail/exam_rank_06" alt="Licence"/>
</p>

<p align="center">
	<img src="https://img.shields.io/github/last-commit/Spidfail/exam_rank_06" alt="Last commit"/>
	<img src="https://img.shields.io/github/issues-pr/Spidfail/exam_rank_06" alt="Pull requests"/>
	<img src="https://img.shields.io/github/issues/Spidfail/exam_rank_06" alt="Issues"/>
	<img src="https://img.shields.io/github/contributors/Spidfail/exam_rank_06" alt="Contributors"/>
</p>

<p align="center">
	<img src="https://img.shields.io/github/followers/Spidfail?style=social" alt="User followers"/>
	<!-- <img src="https://img.shields.io/github/stars/Spidfail?style=social" alt="User followers"/> -->
	<!-- <img src="https://img.shields.io/github/watchers/Spidfail/exam_rank_06?style=social" alt="User followers"/> -->
</p>

## How to test the project
I recommend you to use `netcat` to test your code. It will be the only binary you'll be able to use to in the exam session.

Server command launched with port 8888 :
`gcc -Wall -Wextra -Werror mini_serv.c && ./a.out 8888`

Client command :
`nc localhost 8888`

## Some additional informations
If a client disconnect himself the server and the other clients should be noticed whithout delay.

This version has a shitty technique for reading and sending messages so it can break on one of the test, usually the eighth test so feel free to repush if it fails. It passed on the first try for me.

If you try your code on macOs, the server can crash on the `bind()` function sometimes.
It happens if you relaunch your server too fast on the same port because the port is unavailable for a periode of time after you close the binding.

This code is only meant to help students to build their own version. I encourage anyone to code a better one especially for the message reception and processing part.
