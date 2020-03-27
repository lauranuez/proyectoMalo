DROP DATABASE IF EXISTS partidas;
CREATE DATABASE partidas;
USE partidas;
CREATE TABLE datos_jugador (
    username VARCHAR(60) PRIMARY KEY NOT NULL,
    pass VARCHAR(60) NOT NULL, 
    age INTEGER NOT NULL
)ENGINE=InnoDB;

CREATE TABLE datos_partida (
    ID INTEGER PRIMARY KEY NOT NULL, 
    fecha DATE NOT NULL,
    hora_final TIME NOT NULL,
    duracion_min FLOAT NOT NULL,
    ganador VARCHAR(60) NOT NULL,
    FOREIGN KEY (ganador) REFERENCES datos_jugador(username)
)ENGINE=InnoDB;

CREATE TABLE partida (
    jugador VARCHAR(60) NOT NULL,
    ID_partida INTEGER NOT NULL,
    FOREIGN KEY (jugador) REFERENCES datos_jugador(username),
    FOREIGN KEY (ID_partida) REFERENCES datos_partida(ID)
)ENGINE=InnoDB;

INSERT INTO datos_jugador(username, pass, age) VALUES ('Juan', '123x', 23);
INSERT INTO datos_jugador(username, pass, age) VALUES ('Maria', '543x', 18);
INSERT INTO datos_jugador(username, pass, age) VALUES ('Luis', '232x', 15);
INSERT INTO datos_jugador(username, pass, age) VALUES ('Pedro', '111x',21);

INSERT INTO datos_partida(ID, fecha, hora_final, duracion_min, ganador) VALUES (1,'2020-2-21','13:50:00',15,'Juan');
INSERT INTO datos_partida(ID, fecha, hora_final, duracion_min, ganador) VALUES (2,'2020-9-26','12:50:00',30,'Juan');
INSERT INTO datos_partida(ID, fecha, hora_final, duracion_min, ganador) VALUES (3,'2020-10-2','20:30:00',9,'Luis');
INSERT INTO datos_partida(ID, fecha, hora_final, duracion_min, ganador) VALUES (4,'2020-11-20','00:10:00',7,'Maria');

INSERT INTO partida(jugador,ID_partida) VALUES ('Juan', 1);
INSERT INTO partida(jugador,ID_partida) VALUES ('Maria', 2);
INSERT INTO partida(jugador,ID_partida) VALUES ('Pedro', 1);
INSERT INTO partida(jugador,ID_partida) VALUES ('Luis', 2);
