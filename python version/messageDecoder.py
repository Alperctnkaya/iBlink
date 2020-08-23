import wiringpi
import time

class messageDecoder:
    dictionary={}
    message = None
    closedEyeFrameCount = None
    openEyeFrameCount = None
    shortBlinkBipFlag = None
    longBlinkBipFlag = None
    longerBlinkBipFlag = None
    messageHistory = None
    buzzerPin = None
    ledStart = None
    ledReady = None
    wordCounter = None
    noDetectionCount = None 

    def __init__(self):
        self.initializeDictionary()
        self.closedEyeFrameCount=0
        self.openEyeFrameCount=0
        self.shortBlinkBipFlag = False
        self.longBlinkBipFlag = False
        self.longerBlinkBipFlag = False
        self.noDetectionCount = 0
        self.message=""
        self.messageHistory = []
       	self.buzzerPin = 26
        self.ledReady = 16
        self.ledStart = 21
        self.wordCounter = 0       
        wiringpi.wiringPiSetupGpio()
        wiringpi.pinMode(self.buzzerPin, 1)
        wiringpi.pinMode(self.ledReady, 1)
        wiringpi.pinMode(self.ledStart, 1)
        wiringpi.softToneCreate(self.buzzerPin)
        wiringpi.digitalWrite(self.ledStart, 1)
        wiringpi.digitalWrite(self.ledReady, 0)
        print("decoder created")

    def getState(self, state):
        if (state == 1):
            self.closedEyeFrameCount+=1
            #self.openEyeFrameCount=0

            if ((self.closedEyeFrameCount > 3) and ( not self.shortBlinkBipFlag)):
                self.openEyeFrameCount=0
                self.noDetectionCount=0
                wiringpi.digitalWrite(self.ledStart,0)
                wiringpi.digitalWrite(self.ledReady,0)
                wiringpi.softToneWrite(self.buzzerPin, 500)
                time.sleep(0.05)
                wiringpi.softToneWrite(self.buzzerPin, 0)
                self.shortBlinkBipFlag = True

            elif ((self.closedEyeFrameCount > 8) and ( not self.longBlinkBipFlag)):
                self.openEyeFrameCount=0
                wiringpi.softToneWrite(self.buzzerPin, 450)
                time.sleep(0.05)
                wiringpi.softToneWrite(self.buzzerPin, 0)
                self.longBlinkBipFlag = True
            elif((self.closedEyeFrameCount > 13) and ( not self.longerBlinkBipFlag)):
                self.openEyeFrameCount=0  
                wiringpi.softToneWrite(self.buzzerPin, 400)
                time.sleep(0.05)
                wiringpi.softToneWrite(self.buzzerPin,0)
                self.longerBlinkBipFlag = True
        
        elif (state == 0):
            #self.closedEyeFrameCount = 0
            self.openEyeFrameCount += 1

            if (self.openEyeFrameCount > 2 ):
                self.noDetectionCount=0
                self.closedEyeFrameCount=0
                wiringpi.digitalWrite(self.ledReady,1)
                if (self.longerBlinkBipFlag):
                    self.message+= '2'
                elif (self.longBlinkBipFlag):
                    self.message+= '1'
                elif (self.shortBlinkBipFlag):
                    self.message += '0'

                self.longBlinkBipFlag = False
                self.shortBlinkBipFlag = False
                self.longerBlinkBipFlag = False
        else:
            self.noDetectionCount+=1

        if (self.openEyeFrameCount > 15 or self.noDetectionCount > 15):
            wiringpi.digitalWrite(self.ledStart, 1)
            wiringpi.digitalWrite(self.ledReady, 0)
            self.message = ""
            self.longBlinkBipFlag=False
            self.shortBlinkBipFlag=False
            self.longerBlinkBipFlag=False

        if (len(self.message) == 3):
            self.wordCounter+=1
            wiringpi.digitalWrite(self.ledStart, 1)
            wiringpi.digitalWrite(self.ledReady, 0) 
            self.displayMessage()
            self.messageHistory.append(self.message)
            self.message=""
            if(self.wordCounter==7):
                self.displayDictionary()
                self.wordCounter=0

    def displayMessage(self):
        print(15*" "+"MESAJ: " + self.dictionary[self.message]+"\n")

    def displayDictionary(self):
        j=0
        newline="\n"
        nonewline=""
        ending=""
        for i in self.dictionary:
            if j%2==1:
                ending=newline
            else:
                ending=nonewline
            j+=1

            print(i+"->"+self.dictionary[i]+(20-len(i+"->"+self.dictionary[i]))*" ", end=ending)
        print("\n")

    def initializeDictionary(self):
        self.dictionary = { 
		"000" : "Yardim Çagir",
		"001" : "Evet",
		"002" : "Susadim",
		"010" : "Uykum Var",
		"011" : "Doktor Çagir",
		"012" : "Hayir",
                "020" : "Sicak",
                "021" : "Az",
                "022" : "Isigi Kapat",
		"100" : "Yoruldum",
		"101" : "Aciktim",
		"102" : "Orta",
		"110" : "Pencereyi Aç",
		"111" : "Iyiyim",
		"112" : "Merhaba",
                "120" : "Tuvaletim Var",
                "121" : "Çok",
                "122" : "Soguk",
		"200" : "Isigi Aç",
		"201" : "Agrim Var",
		"202" : "Nasilsin",
		"210" : "Ilacimi Getir",
                "211" : "Iyi Degilim",
                "212" : "TV Aç",
                "220" : "Ustümü Degis",
                "221" : "Müzik Aç",
                "222" : "Disari Çikalim"}

        #self.dictionary = { 
	#	"0000" : "0",
	#	"0001" : "1",
	#	"0010" : "2",
	#	"0011" : "3",
	#	"0100" : "4",
	#	"0101" : "5",
	#	"0110" : "6",
	#	"0111" : "7",
	#	"1000" : "8",
	#	"1001" : "9",
	#	"1010" : "10",
	#	"1011" : "11",
	#	"1100" : "12",
	#	"1101" : "13",
	#	"1110" : "14",
	#	"1111" : "15"   }
