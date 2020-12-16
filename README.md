# feup-rcom-proj2

![](https://img.shields.io/github/license/ctrlMarcio/feup-rcom-proj2)
![](https://img.shields.io/badge/gcc-8.1.0-red)

Resolution proposal for the application used in the second project of the Computer Networks course unit @ FEUP

## Compile

```bash
gcc src/main.c src/util.c -o download
```

## Run

```bash
./download ftp://<username>:<password>@server/path

# for example
./download ftp://rcom:rcom@netlab1.fe.up.pt/pub.txt
```

## License

[MIT](https://opensource.org/licenses/MIT)