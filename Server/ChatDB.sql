
CREATE DATABASE IF NOT EXISTS ChatDB
DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci;

use ChatDB;

CREATE TABLE IF NOT EXISTS users(
    user_id INTEGER PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    isadmin BOOLEAN,
    login VARCHAR(50) NOT NULL UNIQUE
    );

CREATE TABLE IF NOT EXISTS passwords (
    user_id INTEGER PRIMARY KEY,
    password_hash VARCHAR(32) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(user_id)
    );

CREATE TABLE IF NOT EXISTS messages (
    message_id INTEGER PRIMARY KEY AUTO_INCREMENT,
    from_id INTEGER NOT NULL,
    to_id INTEGER NOT NULL,
    message TEXT CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
    FOREIGN KEY (from_id) REFERENCES users(user_id),
    FOREIGN KEY (to_id) REFERENCES users(user_id)
    ) CHARACTER SET utf8;

INSERT INTO users(name, login, isadmin) VALUES
 ('all', 'all', 0),       #служебный пользователь для приема общих сообщений
 ('Bob', 'bob', 0),
 ('Vasya', 'vasa', 1),
 ('Jack', 'black', 0),
 ('Sonya', 'sonka', 0);
   
-- 2360484733 хэш пароля 12345 
INSERT INTO passwords(user_id, password_hash) VALUES
    (1, '1'),
    (2, '2360484733'),
    (3, '2360484733'),
    (4, '2360484733'),
    (5, '2360484733');

INSERT INTO messages(from_id, to_id, message) VALUES 
    (2, 1, 'Hello everybody'),
    (3, 1 ,'Hi bob'),
    (3, 2 ,'Go wolk'),
    (2, 3 ,'Yes ofcouse'),
    (5, 1 ,'Are you her me'),
    (4, 1 ,'May bee');