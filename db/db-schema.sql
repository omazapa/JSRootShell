CREATE DATABASE IF NOT EXISTS rootweb;
GRANT CREATE,INSERT,DELETE,UPDATE,SELECT on rootweb.* to rootdbadmin;
SET PASSWORD FOR rootdbadmin = PASSWORD('rootdbadminpass');
-- USE rootweb;

--
-- Structure for the table `user`
--

CREATE TABLE IF NOT EXISTS `rootweb`.`user` (
  `username` varchar(25) COLLATE utf8_bin NOT NULL,
  `passwd` varchar(41) COLLATE utf8_bin NOT NULL,
  `names` text COLLATE utf8_bin NOT NULL,
  `lastnames` text COLLATE utf8_bin NOT NULL,
  `email` text COLLATE utf8_bin NOT NULL,
  `services` varchar(25) COLLATE utf8_bin NOT NULL,
  `enabled` tinyint(1) NOT NULL,
  PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='This is the table to storage users';


