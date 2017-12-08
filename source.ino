#define senll 4//最左
#define senl 3
#define senc 2//中间
#define senr 5
#define senrr 6//最右
#define senred 9//红外壁障
#define ENA1 10//左电机
#define IN1 7
#define IN2 8
#define ENA2 11//右电机
#define IN3 12
#define IN4 13
int num1, num2, num3, num4, num5;
int p = 0;
int check()
{
	//读到黑色是1
	//要让黑色依次为-2 -1 0 1 2
	//目的是让sum>0时右转
	//sum<0时左转
	//sum==0时直走

	int sum = 0;
	num1 = digitalRead(senll);
	num2 = digitalRead(senl);
	num3 = 0;
	num4 = digitalRead(senr);
	num5 = digitalRead(senrr);

	num1 = num1 * (-2);
	num2 = num2 * (-1);
	num5 = num5 * 2;

	sum = num1 + num2 + num3 + num4 + num5;
	//Serial.print("            sum=");
	//Serial.println(sum,DEC);
	//putout();
	return sum;
}
int checkred()
{
	int re;
	re = digitalRead(senred);
	return  re ;//红外传感器要返回一个值
}
void turnL(int x, int y)
{
	analogWrite(ENA1, x);
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	
	analogWrite(ENA2, x-10);
	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
}
void turnR(int x, int y)
{
	analogWrite(ENA1, x-10);
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);

	analogWrite(ENA2, x);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
}
void runforward(int x)
{
	//直走两边相等
	analogWrite(ENA1, x+2);
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);

	analogWrite(ENA2, x);
	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
}
void work(int judge,int x)
{
	int temp;
	if (judge < 0)
	{
		turnL(x, judge);
		temp = check();
		while (temp < 0)
		{
			turnL(x, temp);
			temp = check();
		}
	}
	if (judge == 0)
	{
		runforward(x);
	}
	if (judge > 0)
	{
		turnR(x, judge);
		temp = check();
		while (temp > 0)
		{
			turnR(x, temp);
			temp = check();
		}
	}

}
void cross(int x)
{
	int temp;
	//思路是先左转 然后直走 然后右转
	turnL(x, 0);
	delay(80);
	//这里不用红外检测的原因是传感器没办法转弯，就没办法一直监控
	runforward(x);
	delay(2250);		//直走的时间 当然和之前不同
				//让小车转出去
	turnR(x, 0);		//右转
	delay(140);		//这个延时应该比之前更久
	runforward(x);
	temp = check();
	while (temp == 0)
		temp = check();
	while (temp > 1)
		temp = check();
	while (temp != 0)
	{
		if (temp > 0)
			turnR(x-25, temp);
		else
			turnL(x-20, temp);
		temp = -check();
	}
	p = 1;
}
void putout()
{
	Serial.print("senll=");
	Serial.println(digitalRead(senll), DEC);
	Serial.print("senl=");
	Serial.println(digitalRead(senl), DEC);
	Serial.print("senc=");
	Serial.println(digitalRead(senc), DEC);
	Serial.print("senr=");
	Serial.println(digitalRead(senr), DEC);
	Serial.print("senrr=");
	Serial.println(digitalRead(senrr), DEC);
	Serial.print("num1=");
	Serial.println(num1, DEC);
	Serial.print("num2=");
	Serial.println(num2, DEC);
	Serial.print("num3=");
	Serial.println(num3, DEC);
	Serial.print("num4=");
	Serial.println(num4, DEC);
	Serial.print("num5=");
	Serial.println(num5, DEC);
//	  delay(2750);
}
void setup()
{
	pinMode(senll, INPUT);
	pinMode(senl, INPUT);
	pinMode(senc, INPUT);
	pinMode(senr, INPUT);
	pinMode(senrr, INPUT);
	pinMode(ENA1, OUTPUT);
	pinMode(ENA2, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
	int x = 245, y = 127;		 //x,y分别代表一个数
	/*
	思路是一直先检测红外然后检测赛道
	壁障的操作看上面的cross()
	*/
	if (!checkred()&&p == 0)//检查到壁障时的工作
		cross(x);
	work(check(),x);

}
