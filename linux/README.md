## non-blocking async Web Server
c++을 사용하여 주소 5개 이상 받고 각기 다른 Json 또는 HTML 출력하는 논블로킹 동기 웹 서버를 구현하였습니다.

---
HW2-1.cpp를 linux에서 실행 후 http://ip주소:12340 으로 들어갔을 때의 결과
  -> 작성된 html을 그대로 보여주는 형태
<img width="893" alt="Screenshot 2024-02-16 at 11 41 51" src="https://github.com/kang-dani/2024_winter_networkStudy/assets/105360184/b5d8c584-5d65-4fa6-9a3d-75138a22c453">

---
HW2-2.cpp를 linux에서 실행 후 http://ip주소:12341 으로 들어갔을 때의 결과
  -> html 파일을 읽어오는 형태
<img width="893" alt="Screenshot 2024-02-16 at 11 45 30" src="https://github.com/kang-dani/2024_winter_networkStudy/assets/105360184/349ef494-883e-4206-8660-183896a7bcd1">

---
HW2-3.cpp를 linux에서 실행 후 http://ip주소:12342 으로 들어갔을 때의 결과
1. http://dani-ubuntu:12342
<img width="849" alt="Screenshot 2024-02-16 at 11 47 02" src="https://github.com/kang-dani/2024_winter_networkStudy/assets/105360184/972a9843-d1ce-4964-813d-fcc85987de47">

2. http://dani-ubuntu:12342/linuxPage
   -> html 파일을 읽어오는 형태
<img width="893" alt="Screenshot 2024-02-16 at 11 48 21" src="https://github.com/kang-dani/2024_winter_networkStudy/assets/105360184/92d7ac7d-376d-4a2d-b627-277c01d2c10e">

4. http://dani-ubuntu:12342/hi
   -> html 파일을 읽어오는 형태
<img width="893" alt="Screenshot 2024-02-16 at 11 59 56" src="https://github.com/kang-dani/2024_winter_networkStudy/assets/105360184/94d2868f-a205-4d2a-b87d-ee9ece0ee1b2">

6. http://dani-ubuntu:12342/api
   -> 리스트 형태의 json 값을 직접 전달받아서 보여주는 형태
<img width="893" alt="Screenshot 2024-02-16 at 11 50 20" src="https://github.com/kang-dani/2024_winter_networkStudy/assets/105360184/931c98f3-3939-4702-aa32-bd901dbe1e37">
