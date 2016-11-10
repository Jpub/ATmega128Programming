// 포트 D의 핀에 할당된 핀 번호
int pins[] = {16, 17, 18, 19, 20, 21, 22, 23};
boolean state = false;      // LED 상태

void setup() {
  for (int i = 0; i < 8; i++) { // 각 핀을 출력으로 설정
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  state = !state;       // LED 상태 반전
  for (int i = 0; i < 8; i++) { // LED 점멸
    digitalWrite(pins[i], state);
  }

  delay(1000);        // 1초 대기
}
