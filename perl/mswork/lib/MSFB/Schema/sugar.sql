CREATE TABLE ms_sugar_question(
usr_id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
question text,
order_by integer(11),
active integer(11) default 1 -- 1 is active. 0 is non-active
);


CREATE TABLE ms_sugar_answer(
usr_id INT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
description text,
min_yes integer(11),
max_yes integer(11),
user_count integer(11),
active integer(11) default 1 -- 1 is active. 0 is non-active
);

CREATE TABLE ms_sugar_user(
fb_uid integer(11) UNSIGNED NOT NULL PRIMARY KEY,
sugar_category integer(11),
quiz_answer varchar(255),
joined integer(1) default 0, -- 1 is joined. 0 isn't join
seeking integer(11), 
email varchar(255), 
im varchar(255), 
photo varchar(255), 
description text,

create_date datetime,
join_date datetime
);

CREATE TABLE ms_sugar_featured(
fb_uid integer(11) UNSIGNED NOT NULL PRIMARY KEY,
create_date datetime 
);

insert ms_sugar_question(question, order_by)
values ('Do you take pre-Marital sex easy?', 1),
('Do you take extra-Marital sex easy?', 2),
('Is it human instinct to be attracted to beauty as it is to be attracted to wealth and power?', 3),
('Do you love to treated like a king or queen?', 4),
('Are you generous to support or care?', 5),
('Is it human nature for people to want younger and more attractive partners?', 6),
('Is it natural for younger men & women to seek out more experienced, sophisticated partners, specifically those who have the means of providing them with comforts and luxuries?', 7),
('Are you mature and rich or young but in financial difficulty?', 8),
('Are you interested in a discreet and mutually beneficial relationship?', 9),
('Are you want good living and finer things in life and also share with others?', 10);


insert ms_sugar_answer(description, min_yes, max_yes, user_count)
values ('Yes You like Sugardaddy Lifestyle.', 10, 100, 0),
 ('You are absolutely interested in Sugardaddy Lifestyle.', 8, 9, 0),
 ('You are probably interested in Sugardaddy Lifestyle.', 5, 7, 0),
 ('You are probably not interested in Sugardaddy Lifestyle.', 3, 4, 0),
('Yes You are not interested in Sugardaddy Lifestyle.', 1, 2, 0),
('You hate Sugardaddy Lifestyle.', -1, 0, 0);
