create table friends(
    user_id int(10),
    friend_id int(10),
    roletype int(10),
    weight int(10)
);

insert into friends(user_id, friend_id, roletype, weight ) values(1,2, 2, 1);
insert into friends(user_id, friend_id, roletype, weight) values(2,1, 1, 1);


insert into friends(user_id, friend_id, roletype, weight ) values(2, 3, 4, 1);