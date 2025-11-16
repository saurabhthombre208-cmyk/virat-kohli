#include<P18F4520.h>
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN  PORTCbits.RC3
#define data_line PORTD
void delay(int);
void lcd_data(unsigned char);
void lcd_cmd(unsigned char);
void lcd_init(void);
void lcd_string(unsigned char *);
void ADC_init(void);
unsigned int ADC_read(void);
void main()
{
int i;
unsigned int val;
unsigned char a[8]="fan off";
unsigned char b[8]="fan on"; 
float temp;

TRISC=0X00;
T2CON=0X01;
CCP1CON=0X0C;
PR2=124;
T2CONbits.TMR2ON=1;


lcd_init();
ADC_init();

while(1)
{
val=ADC_read();
temp=val*0.489;
if(temp<25)
{
CCPR1L=0;
lcd_cmd(0xc0);
lcd_string(a);
delay(10);
}
else if(temp>35)
{
CCPR1L=124;
lcd_cmd(0xc0);
lcd_string(b);
}
else 
{
CCPR1L=60;
lcd_cmd(0xc0);
lcd_string(b);
}
val=temp;
lcd_cmd(0x80);
lcd_data((val/10)+'0');
lcd_data((val%10)+'0');
lcd_data('.');
temp =temp-val;
temp=temp*100;
val=temp;
lcd_data((val/10)+'0');
lcd_data((val%10)+'0');
}
}

void ADC_init(void)
{
ADCON0=0X19;
ADCON1=0X00;
ADCON2=0XA4;
}
unsigned int ADC_read(void)
{
unsigned int adc;
ADCON0bits.GO=1;
while(ADCON0bits.GO!=0);
adc=ADRESH;
adc=adc<<8;
adc=adc|ADRESL;
return adc;
}



void delay(int t)
{
int i,j;
for(i=0;i<t;i++){
for(j=0;j<200;j++);
}

}
void lcd_cmd(unsigned char c)
{
data_line = c;
RS=0;
RW=0;
EN=1;
delay(10);
EN=0;
}
void lcd_data(unsigned char c)
{
data_line = c;
RS=1;
RW=0;
EN=1;
delay(10);
EN=0;
}
void lcd_init(void)
{
TRISC=0X00;
TRISD=0X00;
lcd_cmd(0x30);
lcd_cmd(0x38);
lcd_cmd(0x01);
lcd_cmd(0x80);
lcd_cmd(0x0F);
}
void lcd_string(unsigned char *p)
{
while(*p!='\0')
{
lcd_data(*p++);
}


}



