--------------------------------------------
-- SQL queries for the
-- creation of the tables
-- [Fr_de] and [Cat_gram] in the
-- [vocabulary] database.
-- Created: 7.1.2016
-- Author: Thomas Lugrin
-- Project: Einfach fließend!
--------------------------------------------


---------------------------------
-- Create [vocabulary] database
---------------------------------

CREATE DATABASE vocabulary CHARACTER SET 'utf8';


-------------------------
-- Create [Fr_de] table
-------------------------

CREATE TABLE Fr_de (
     id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
     word_fr_1 VARCHAR(50) NOT NULL,
     word_fr_2 VARCHAR(50),
     word_fr_3 VARCHAR(50),
     word_de_1 VARCHAR(50) NOT NULL,
     word_de_2 VARCHAR(50),
     word_de_3 VARCHAR(50),
     word_type TINYINT UNSIGNED NOT NULL,
     irreg_verb BOOLEAN,
     gender_fr TINYINT UNSIGNED,
     gender_de TINYINT UNSIGNED,
     ex_fr_1 TEXT,
     ex_fr_2 TEXT,
     ex_fr_3 TEXT,
     ex_de_1 TEXT,
     ex_de_2 TEXT,
     ex_de_3 TEXT,
     created DATE NOT NULL,
     modified DATE NOT NULL,
     last_asked DATE,
     nbr_asked SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     nbr_wrong SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     last_wrong BOOLEAN NOT NULL DEFAULT FALSE,
     nbr_last_wrong SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     nbr_last_correct SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     PRIMARY KEY (id)
     )
     ENGINE=INNODB;


---------------------------
-- Description of [Fr_de]
---------------------------

+------------------+----------------------+------+-----+---------+----------------+
| Field            | Type                 | Null | Key | Default | Extra          |
+------------------+----------------------+------+-----+---------+----------------+
| id               | smallint(5) unsigned | NO   | PRI | NULL    | auto_increment |
| word_fr_1        | varchar(50)          | NO   |     | NULL    |                |
| word_fr_2        | varchar(50)          | YES  |     | NULL    |                |
| word_fr_3        | varchar(50)          | YES  |     | NULL    |                |
| word_de_1        | varchar(50)          | NO   |     | NULL    |                |
| word_de_2        | varchar(50)          | YES  |     | NULL    |                |
| word_de_3        | varchar(50)          | YES  |     | NULL    |                |
| word_type        | tinyint(3) unsigned  | NO   |     | NULL    |                |
| irreg_verb       | tinyint(1)           | YES  |     | NULL    |                |
| gender_fr        | tinyint(3) unsigned  | YES  |     | NULL    |                |
| gender_de        | tinyint(3) unsigned  | YES  |     | NULL    |                |
| ex_fr_1          | text                 | YES  |     | NULL    |                |
| ex_fr_2          | text                 | YES  |     | NULL    |                |
| ex_fr_3          | text                 | YES  |     | NULL    |                |
| ex_de_1          | text                 | YES  |     | NULL    |                |
| ex_de_2          | text                 | YES  |     | NULL    |                |
| ex_de_3          | text                 | YES  |     | NULL    |                |
| created          | date                 | NO   |     | NULL    |                |
| modified         | date                 | NO   |     | NULL    |                |
| last_asked       | date                 | YES  |     | NULL    |                |
| nbr_asked        | smallint(5) unsigned | NO   |     | 0       |                |
| nbr_wrong        | smallint(5) unsigned | NO   |     | 0       |                |
| last_wrong       | tinyint(1)           | NO   |     | 0       |                |
| nbr_last_wrong   | smallint(5) unsigned | NO   |     | 0       |                |
| nbr_last_correct | smallint(5) unsigned | NO   |     | 0       |                |
+------------------+----------------------+------+-----+---------+----------------+

-------------------------
-- Create [Fr_sh] table
-------------------------
CREATE TABLE Fr_sh (
     id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
     word_fr_1 VARCHAR(50) NOT NULL,
     word_fr_2 VARCHAR(50),
     word_fr_3 VARCHAR(50),
     word_sh_1 VARCHAR(50) NOT NULL,
     word_sh_2 VARCHAR(50),
     word_sh_3 VARCHAR(50),
     word_type TINYINT UNSIGNED NOT NULL,
     irreg_verb BOOLEAN,
     gender_fr TINYINT UNSIGNED,
     gender_sh TINYINT UNSIGNED,
     ex_fr_1 TEXT,
     ex_fr_2 TEXT,
     ex_fr_3 TEXT,
     ex_sh_1 TEXT,
     ex_sh_2 TEXT,
     ex_sh_3 TEXT,
     created DATE NOT NULL,
     modified DATE NOT NULL,
     PRIMARY KEY (id)
     )
     ENGINE=INNODB;
     
CREATE TABLE Username_frsh (
     id SMALLINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
     word_id SMALLINT UNSIGNED,
     last_asked DATE,
     nbr_asked SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     nbr_wrong SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     last_wrong BOOLEAN NOT NULL DEFAULT FALSE,
     nbr_last_wrong SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     nbr_last_correct SMALLINT UNSIGNED NOT NULL DEFAULT 0,
     CONSTRAINT fk_word_id
	FOREIGN KEY (word_id)
	REFERENCES Fr_sh(id)
	ON DELETE CASCADE
     )
     ENGINE=INNODB;
     
ALTER TABLE Username_frsh MODIFY word_id SMALLINT UNSIGNED NOT NULL;
ALTER TABLE Username_frsh ADD UNIQUE index_word_id (word_id);


----------------------------
-- Create [Cat_gram] table
----------------------------
CREATE TABLE Cat_gram (
     id TINYINT UNSIGNED NOT NULL AUTO_INCREMENT,
     cat VARCHAR(12) NOT NULL,
     PRIMARY KEY (id)
     )
     ENGINE=INNODB;


------------------------------
-- Description of [Cat_gram]
------------------------------
+-------+---------------------+------+-----+---------+----------------+
| Field | Type                | Null | Key | Default | Extra          |
+-------+---------------------+------+-----+---------+----------------+
| id    | tinyint(3) unsigned | NO   | PRI | NULL    | auto_increment |
| cat   | varchar(12)         | NO   |     | NULL    |                |
+-------+---------------------+------+-----+---------+----------------+


-----------------------
-- Create [Gdr] table
-----------------------
CREATE TABLE Gdr (
      id TINYINT UNSIGNED NOT NULL AUTO_INCREMENT,
      cat VARCHAR(2) NOT NULL,
      PRIMARY KEY (id)
      )
      ENGINE=INNODB;


------------------------------
-- Description of [Gdr]
------------------------------
+-------+---------------------+------+-----+---------+----------------+
| Field | Type                | Null | Key | Default | Extra          |
+-------+---------------------+------+-----+---------+----------------+
| id    | tinyint(3) unsigned | NO   | PRI | NULL    | auto_increment |
| cat   | varchar(2)          | NO   |     | NULL    |                |
+-------+---------------------+------+-----+---------+----------------+
