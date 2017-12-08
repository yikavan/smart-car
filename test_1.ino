#define senll 4//����
#define senl 3
#define senc 2//�м�
#define senr 5
#define senrr 6//����
#define senred 9//������� ����ط�Ҫ�����Լ�����
#define ENA1 10//����
#define IN1 7
#define IN2 8
#define ENA2 11//�ҵ��
#define IN3 12
#define IN4 13
int num1, num2, num3, num4, num5;
int p = 0;
int check()
{
	//������ɫ��1
	//Ҫ�ú�ɫ����Ϊ-2 -1 0 1 2
	//Ŀ������sum>0ʱ��ת
	//sum<0ʱ��ת
	//sum==0ʱֱ��

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
			//���ǲ�һ����ô�����ֵȻ�������ֵ����
	return  re ;//���⴫����Ҫ����һ��ֵ
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
	//ֱ���������
	analogWrite(ENA1, x+2);
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);

	analogWrite(ENA2, x);
	digitalWrite(IN3, HIGH);
	digitalWrite(IN4, LOW);
//	Serial.println("                runforward");
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
	int y=350;//��y��������ʱ
	int temp;
	//˼·������ת Ȼ��ֱ�� Ȼ����ת
	turnL(x, 0);
	delay(80);
					//���ﲻ�ú������ԭ���Ǵ�����û�취ת�䣬��û�취һֱ���
	runforward(x);
	delay(2250);		//ֱ�ߵ�ʱ�� ��Ȼ��֮ǰ��ͬ
						//��С��ת��ȥ
	turnR(x, 0);		//��ת
	delay(140);		//�����ʱӦ�ñ�֮ǰ����
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
	int x = 245, y = 127;		 //x,y�ֱ����һ����
	/*
	˼·��һֱ�ȼ�����Ȼ��������
	���ϵĲ����������cross()
	*/
	if (!checkred()&&p == 0)				//��鵽����ʱ�Ĺ���
		cross(x);
	work(check(),x);

}