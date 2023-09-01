<h1 align="center">
	<img alt="badge cpp" src="./img/cpp_dark.svg" />
 </h1>

 <div align="center">

[![NPM](https://img.shields.io/npm/l/react)](https://github.com/nandajfa/CPP/blob/main/LICENSE)

 </div>

### 🗣️ About

> _This project is about writing your ow HTTP server._

####

-> HTTP is one of the most used protocols on the internet. Knowing its arcane will be useful, even if you won’t be working on a website.

---

### Index

- [HTTP Intro](#introduction)
- [HTTP Messages](#http-messages)
- [HTTO Methods](#http-methods)
- [References 📌](#references)
- [Skills 📄](#skills)
- [Author Info ✒️](#author)

## Introduction

The Hypertext Transfer Protocol (HTTP) is an application protocol for distributed, collaborative, hypermedia information systems.
The primary function of a web server is to store, process, and deliver web pages to clients. The communication between client and server takes place using the Hypertext Transfer Protocol (HTTP).

An HTTP web server is a software application that listens for and responds to HTTP requests from clients (such as web browsers). The main purpose of a web server is to host web content and make it available to users over the internet.

HTTP consists of requests and responses. When a client (such as a web browser) wants to retrieve a webpage from a server, it sends an HTTP request to the server. The server then processes the request and sends back an HTTP response.

## HTTP Messages

HTTP messages are how data is exchanged between a server and a client. There are two types of messages: requests sent by the client to trigger an action on the server, and responses, the answer from the server.

#### HTTP Requests

- Start line

HTTP requests are messages sent by the client to initiate an action on the server. Their start-line contain three elements:

    1. An HTTP method, a verb (like GET, PUT or POST) or a noun (like HEAD or OPTIONS), that describes the action to be performed.
    2. The request target, usually a URL, or the absolute path of the protocol, port, and domain are usually characterized by the request context.
    3. The HTTP version, which defines the structure of the remaining message, acting as an indicator of the expected version to use for the response.

- Request header

Example after a `GET` request:

```HTTP
GET /home.html HTTP/1.1
Host: developer.mozilla.org
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) Gecko/20100101 Firefox/50.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: https://developer.mozilla.org/testpage.html
Connection: keep-alive
Upgrade-Insecure-Requests: 1
If-Modified-Since: Mon, 18 Jul 2016 02:36:04 GMT
If-None-Match: "c561c68d0ba92bbeb8b0fff2a9199f722e3a621a"
Cache-Control: max-age=0
```

- Body

Bodies can be broadly divided into two categories:

1. Single-resource bodies, consisting of one single file, defined by the two headers: Content-Type and Content-Length.
2. Multiple-resource bodies, consisting of a multipart body, each containing a different bit of information. This is typically associated with HTML Forms.

#### HTTP Responses

- Status line

The start line of an HTTP response, called the status line, contains the following information:

1. The protocol version, usually HTTP/1.1.
2. A status code, indicating success or failure of the request. Common status codes are 200, 404, or 302
3. A status text. A brief, purely informational, textual description of the status code to help a human understand the HTTP message.

A typical status line looks like: HTTP/1.1 404 Not Found.

- Responde header

A response header is an HTTP header that can be used in an HTTP response and that doesn't relate to the content of the message. Response headers, like Age, Location or Server are used to give a more detailed context of the response.

Example after a `GET` request:

```HTTP
200 OK
Access-Control-Allow-Origin: *
Connection: Keep-Alive
Content-Encoding: gzip
Content-Type: text/html; charset=utf-8
Date: Mon, 18 Jul 2016 16:06:00 GMT
Etag: "c561c68d0ba92bbeb8b0f612a9199f722e3a621a"
Keep-Alive: timeout=5, max=997
Last-Modified: Mon, 18 Jul 2016 02:36:04 GMT
Server: Apache
Set-Cookie: mykey=myvalue; expires=Mon, 17-Jul-2017 16:06:00 GMT; Max-Age=31449600; Path=/; secure
Transfer-Encoding: chunked
Vary: Cookie, Accept-Encoding
X-Backend-Server: developer2.webapp.scl3.mozilla.com
X-Cache-Info: not cacheable; meta data too large
X-kuma-revision: 1085259
x-frame-options: DENY
```

- Body

Bodies can be broadly divided into three categories:

1. ingle-resource bodies, consisting of a single file of known length, defined by the two headers: Content-Type and Content-Length.
2. Single-resource bodies, consisting of a single file of unknown length, encoded by chunks with Transfer-Encoding set to chunked.
3. Multiple-resource bodies, consisting of a multipart body, each containing a different section of information. These are relatively rare.

<details>
    <summary>Toggle Switch</summary>
    Foldable Content[enter image description here][1]
</details>

## HTTP Methods

| Method   |                                                         Description                                                         | Possible body |
| -------- | :-------------------------------------------------------------------------------------------------------------------------: | ------------- |
| `GET`    |      The GET method requests a representation of the specified resource. Requests using GET should only retrieve data.      | NO            |
| `POST`   | The POST method submits an entity to the specified resource, often causing a change in state or side effects on the server. | YES           |
| `DELETE` |                                      The DELETE method deletes the specified resource.                                      | YES           |
| `PUT`    |            The PUT method replaces all current representations of the target resource with the request payload.             | YES           |
| `HEAD`   |               The HEAD method asks for a response identical to a GET request, but without the response body.                | NO            |

## References

- [Summary](https://www.youtube.com/watch?v=TnrQMtxPeEg)
- [C++ course](https://www.youtube.com/playlist?list=PLx4x_zx8csUjczg1qPHavU1vw1IkBcm40)
- [Notion 42](https://soraianovaes.notion.site/CPP-Piscine-de222a1b2db34121a7228101c61b7c87)
- [Wiki - cpp](https://github.com/qingqingqingli/CPP/wiki)

## Skills

- Unix
- Rigor
- Network & system administration
- Object-oriented programming

## Author

<table >
  <tr>
    <td align="center"><a href="https://github.com/marcocslima"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/3109487?v=4" width="100px;" alt=""/><br /><sub><b>Marco Lima</b></sub></a><br />
    <td align="center"><a href="https://github.com/Mitsu325"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/56398748?v=4" width="100px;" alt=""/><br /><sub><b>Patrícia Mitsuko</b></sub></a><br />
        <td align="center"><a href="https://www.linkedin.com/in/jessica-fernanda-programadora"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/80687429?v=4" width="100px;" alt=""/><br /><sub><b>Jessica Fernanda</b></sub></a><br />

  </tr>
</div>

<div align="right">
  <b><a href="#index">↥ back to top</a></b>
</div>
</br>
