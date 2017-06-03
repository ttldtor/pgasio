/*
    Copyright 2017, Kirit Sælensminde. http://www.kirit.com/pgasio/
*/
/// # select
///
/// Select data from Postgres.


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
    });
    /// Finally run the coroutine in this thread
    ios.run();

    return 0;
}

