CREATE TABLE Grocery_Lists (
    list_id SMALLINT NOT NULL,
    title TINYTEXT NOT NULL,
    PRIMARY KEY (list_id)
);

CREATE TABLE Recipe (
    dish TINYTEXT NOT NULL,
    cook_time INT NOT NULL,
    servings TINYINT NOT NULL,
    serving_mult TINYINT NOT NULL,
    PRIMARY KEY (dish)
);

CREATE TABLE Ingredient (
    name TINYTEXT NOT NULL,
    category TINYTEXT,
    measure_unit TINYTEXT NOT NULL,
    expiration DATE,
    PRIMARY KEY (name)
);

CREATE TABLE Requires (
    dish TINYTEXT NOT NULL,
    ingredient_name TINYTEXT NOT NULL,
    amt_needed INT NOT NULL,
    PRIMARY KEY (dish, ingredient_name),
    FOREIGN KEY (dish) REFERENCES Recipe(dish),
    FOREIGN KEY (ingredient_name) REFERENCES Ingredient(name)
);

CREATE TABLE Lists (
    list_id SMALLINT NOT NULL,
    ingredient_name TINYTEXT NOT NULL,
    PRIMARY KEY (list_id, ingredient_name),
    FOREIGN KEY (list_id) REFERENCES Grocery_Lists(list_id),
    FOREIGN KEY (ingredient_name) REFERENCES Ingredient(name)
);

CREATE TABLE User (
    username VARCHAR(10) NOT NULL,
    password TINYTEXT NOT NULL,
    email TINYTEXT NOT NULL,
    first_name TINYTEXT NOT NULL,
    last_name TINYTEXT NOT NULL,
    PRIMARY KEY (username)
);

CREATE TABLE Auth_User (
    username TINYTEXT NOT NULL,
    owner_username TINYTEXT NOT NULL,
    password TINYTEXT NOT NULL,
    email TINYTEXT NOT NULL,
    first_name TINYTEXT NOT NULL,
    last_name TINYTEXT NOT NULL,
    PRIMARY KEY (username),
    FOREIGN KEY (owner_username) REFERENCES User(username)
);

CREATE TABLE Has (
    ingredient_name TINYTEXT NOT NULL,
    username TINYTEXT NOT NULL,
    PRIMARY KEY (ingredient_name, username),
    FOREIGN KEY (ingredient_name) REFERENCES Ingredient(name),
    FOREIGN KEY (username) REFERENCES User(username)
);
