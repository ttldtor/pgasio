/*
    Copyright 2017, Kirit Sælensminde. http://www.kirit.com/pgasio/
*/
/// # connect
///
/// Connect to postgres and check a setting.


#include <cstdlib> // for std::getenv
#include <iostream>

#include <pgasio/query.hpp>


int main() {
    /// The parameters we need to use.
    const char *user = std::getenv("LOGNAME");
    const char *database = nullptr;
    const char *path = "/var/run/postgresql/.s.PGSQL.5432";

    /// We need a container for the coroutines
    boost::asio::io_service ios;
    /// Spawn a coroutine to perform our database operations with
    boost::asio::spawn(ios, [&](auto yield) {
        /// Open the connection to postgres.
        auto cnx = pgasio::handshake(
            pgasio::unix_domain_socket(ios, path, yield),
            user, database, yield);
        std::cout << "Connected to " << path << " as " << user << std::endl;
        for ( const auto &s : cnx.settings ) {
            std::cout << s.first << ": " << s.second << std::endl;
        }

        auto setting = cnx.settings.find("client_encoding");
        if ( setting == cnx.settings.end() ) {
            std::cout << "\nclient_encoding not found" << std::endl;
            std::exit(1);
        } else if ( setting->second != "UTF8" ) {
            std::cout << "\nclient_encoding must be UTF8" << std::endl;
            std::exit(1);
        }
    });
    /// Finally run the coroutine in this thread
    ios.run();

    return 0;
}

