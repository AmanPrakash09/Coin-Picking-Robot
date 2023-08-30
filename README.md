# Project2

Robot Description: 

The robot we designed has all the capabilities required for the project (detecting/picking 20 coins and then stopping, detecting and avoiding perimeters). 

Our team added the following additional functionality:

Website:
The website was created to give the user more information about the robot, and gives the user access to some videos as well as 
the github repository on request

The website was created using the following technologies:
 - HTML (Hyper text Markup Language)
 - CSS (Cascading Style Sheets)
 - Bootstrap v5

Discord Bot, Screen Scraping notifier:

Here, we created two separate functionalities that together accomplish the goal of notifying members of our team when a coin has been picked up.
The workflow is as follows:

 - The putty terminal shows the number of coins that are picked up by the robot and is displayed using the c code accompanying the robot logic. 
The number of coins displayed on the putty terminal is scraped using a pyautogui python screen scraper. It is built such that it gathers the data 
from the putty terminal, and transfers the data to a discord channel made for the purposes of this functionality. Every single time data (a number) is 
shown in the discord channel, the discord bot parses it.

 - The discord bot was developed in Python, named bot.py and utilizes the discord developer library and a load environment library that 
can interact live with a functioning discord server. The python bot script uses environment variables to directly associate itself with a server 
and set of users (Project 2 members), allowing any member to interact with the bot without having access to the code. The script runs through the parameters
and conditions upon the input of a message into a server channel. The bot only begins activity when the python script is run and loaded on a terminal.
If a non-integer number or unrealistic number is written, the bot is prepared to respond with a message indicating that this input is not valid for 
coin counting. Data from putty is received on a frequent interval and often enough, the coin count received is the same number sent repeatedly 
since the vehicle spends time travelling between coin pickups. To counter this, the bot was programmed to only record repeated numerical inputs once 
and ignore any inputs below the current coin count. When no coins are collected, the bot receives input '0' and responds with a comical message indicating
that no coins have been accumulated. As the coin count increases, new and unique messages are sent from the bot indicating each new coin counted, 
and notifying each member in the team when a coin is added. The bot has the power to send messages and gifs in response to coin pickups, 
delete redundant putty data in the server, and notify each member of the robot's coin pickup activity.


Music using 8051 microcontroller

 - Plays Tom and Jerry Music using the CEM-1302 speaker and 8051  Arm Assembly code.

LCD Coin Cointer using 8051 microcontroller

The LCD is connencted to an 8051 microcontroller that is interfaced with an MCP3008 and LM4040 that detects voltage coming from pin 11 (electromagnet) of the
LPC824. Every time the electromagnet is activated after detecting a coin, a logical 1 (3.3 V) is sent to the 8051 that runs code in C to increment a counter
displayed on the LCD.






Complete list of Languages and Technologies used for this project:
 - C
 - 8051 Assembly
 - HTML
 - CSS
 - Bootstrap v5
 - Python
 - Pyautogui (Python)
