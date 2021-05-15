
import time,datetime 
import telepot 
import telepot.loop import  MessageLoop
import serial

ser = serial.Serial('/dev/ttyACM0',9600)
now=datetime.datetime.now()
def action(msg):
   chat_id=msg(“chat”,”msg”)
    command=msg(“text”)
   print(“received %s “, command)
  if “show” in command
     read_serial=ser.readline()
     message=read_serial
 else 
   message=”tchau“
telegram_bot.sendMessage(chat_id,message); 
}
Telegram_bot=teleport.Bot(1884948805:AAF_a7CZWBFrT1qq8MKyKibqDQJFzg1FD4I)
Print(telegram_bot.getMe())
MessageLoop(telegram_bot,action).run_as_thread()
Print(“Up and Running ….”);
while 1:
   time.sleep(10)
