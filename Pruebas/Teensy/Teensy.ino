const size_t dataLength = 4;
int data[4] = {2, 2,4,4};

void setup()
{
  Serial.begin(115200);
  Serial1.begin(57600);
  Serial5.begin(74880);
  pinMode(13, OUTPUT);
} 

void loop()
{
 
  //Serial5.write((byte*)data, dataLength * sizeof(data[0]));
  if(Serial1.available())
  {
  digitalWrite(13, HIGH); 
  Serial.println(Serial1.read()); 
  }
  Serial1.write(2);
}