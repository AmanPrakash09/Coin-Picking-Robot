import os
import random

import discord
from dotenv import load_dotenv

load_dotenv()
TOKEN = os.getenv('DISCORD_TOKEN')

client = discord.Client()

count = 0
maxcoin = 0

@client.event
async def on_message(message):
    global count
    global maxcoin
    if message.author == client.user:
        return

    test_quotes = [
        'Wow more coins yo! @everyone https://c.tenor.com/vpva0KfiifsAAAAC/rich-money.gif',
        'My money is up baby @everyone https://c.tenor.com/KDiNSmRbmNsAAAAC/johnny-test-bling-bling-boy.gif',
        'MORE COINS, MORE PROBLEMS @everyone https://i.gifer.com/B3bX.gif',
        'Keep em coming @everyone https://c.tenor.com/upZjDOWgvX8AAAAC/spongebob-too-easy.gif',
        'The only thing Im tripping on is this bag @everyone https://c.tenor.com/SVYrWWTq5VIAAAAC/counting-money-im-rich.gif',
        'Oh we rich rich @everyone https://c.tenor.com/xRvB68C7T2EAAAAM/attitude-confident.gif',
        'Which city should I buy this week? @everyone https://c.tenor.com/kOugL3xNMoAAAAAC/incredibles-glasses.gif',
        'Im built different @everyone https://c.tenor.com/O7NOWzsUw18AAAAM/pumping-iron-arnold-schwarzenegger.gif',
        'Im never caught sleeping on that bag @everyone https://c.tenor.com/wu0z3nw60gcAAAAC/breaking-bad-money.gif'
    ]
    
    fail_quotes = [
        'No more coins? https://c.tenor.com/UPrhVQ3WFPgAAAAM/spare-change.gif',
        'Damn ur broke',
        'Still no coins man https://media0.giphy.com/media/TKvHkcnbtGgKxQwlqu/giphy.gif?cid=ecf05e47y5k53uznz1elmhdi60mr8j4pzq83d4460k1r74ql&rid=giphy.gif&ct=g',
        'Le coins are not present',
        'https://i.imgflip.com/6bj387.jpg'    
    ]

    bad_inputs = [
        'L input lmao https://i0.wp.com/insidejamarifox.com/wp-content/uploads/2020/12/m2x22n.gif?resize=480%2C270&ssl=1',
        'THATS NOT MONEY NUMBERS MAN https://c.tenor.com/w47Dk_0uHwYAAAAC/star-wars-scared.gif',
        'Who let you speak',
        'https://media3.giphy.com/media/ujZtlj1Y7wXyE/giphy.gif'
    ]

    kick = 0
    #if(message.channel.id == "936380896347582528"): 
    try:
        chatnum = int(message.content)
        if chatnum >= 1 and chatnum > maxcoin:
            response = random.choice(test_quotes)
            await message.channel.send(response)
            maxcoin = chatnum
            kick = 1
        if chatnum == 0 and count == 0:
            response = random.choice(fail_quotes)
            await message.channel.send(response)
            count = 1
            kick = 1
        if chatnum == 0 and count > 0 and kick == 0:
            await message.delete()
        if chatnum >= 1 and chatnum <= maxcoin and kick == 0:
            await message.delete()
    except ValueError:
        response = random.choice(bad_inputs)
        await message.channel.send(response)
    
client.run(TOKEN)