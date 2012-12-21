var settings = {}

settings.web = {};
settings.database = {};

// Database path
settings.database.path = "./isk-db";

// Save database on server stop
settings.database.save_on_exit = true;

// Autosave database regulary
settings.database.autosave = true;

// Autosave interval in minits
settings.database.autosave_interval = 15;

settings.web.host = "127.0.0.1";
settings.web.port = 31129;

module.exports = settings;