FROM    eclipse-mosquitto:2.0.18

WORKDIR /mosquitto/config
COPY    mosquitto.conf .
COPY    passwd .

RUN     mosquitto_passwd -U passwd
RUN     chmod 0700 passwd

EXPOSE  8883 8884
