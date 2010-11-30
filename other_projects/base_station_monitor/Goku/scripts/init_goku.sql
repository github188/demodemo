BEGIN;
CREATE TABLE `base_station` (
    `uuid` varchar(10) NOT NULL PRIMARY KEY,
    `connectionStatus` varchar(10) NOT NULL,
    `groupName` varchar(50) NOT NULL,
    `manageServer` varchar(50) NOT NULL,
    `locationId` varchar(50) NOT NULL,
    `alarmStatus` varchar(50) NOT NULL,
    `devType` integer NOT NULL,
    `lastActive` datetime NOT NULL,
    `lastUpdate` datetime NOT NULL,
    `createDate` datetime NOT NULL
)
;
CREATE TABLE `user_account` (
    `name` varchar(20) NOT NULL PRIMARY KEY,
    `password` varchar(50) NOT NULL,
    `display` varchar(50) NOT NULL
)
;
CREATE TABLE `user_group` (
    `name` varchar(20) NOT NULL PRIMARY KEY,
    `isAdmin` integer NOT NULL
)
;
COMMIT;

