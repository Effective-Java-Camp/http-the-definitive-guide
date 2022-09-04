# Nginx에서 HTTP/2.0 적용하기

- [HTTP/2.0 NGINX 적용하기](https://tecoble.techcourse.co.kr/post/2021-09-20-http2/)
- [HTTP/2.0 + NGINX + Dockerizing](https://da-nyee.github.io/posts/network-how-to-set-up-nginx-with-http2.0/)

```
server {
    # ssl설정을 해주고, http2를 적어주면 쉽게 설정할 수 있다.
    listen 443 ssl http2;

    ssl_certificate ssl/certificate.pem;
    ssl_certificate_key ssl/key.pem;

    root /var/www/html;

    # index.html에 요청이 왔을 때, style.css, main.js, image.jpg 파일도 함께 서버 푸시된다.
    location = /index.html {
        http2_push /style.css;
        http2_push /main.js;
        http2_push /image.jpg;
    }
}
```