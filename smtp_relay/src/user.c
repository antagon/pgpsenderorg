#include <string.h>
#include <sqlite3.h>

#include "user.h"

int
user_exists (sqlite3 *sqlite, const char *user)
{
	sqlite3_stmt *sqlite_stmt;
	const char *query = "SELECT 1 FROM user WHERE name = ?;";
	int rval;

	rval = sqlite3_prepare (sqlite, query, -1, &sqlite_stmt, 0);

	if ( rval != SQLITE_OK )
		return 0;
	
	rval = sqlite3_bind_text (sqlite_stmt, 1, user, strlen (user), SQLITE_STATIC);

	if ( rval != SQLITE_OK )
		return 0;
	
	rval = sqlite3_step (sqlite_stmt);

	sqlite3_finalize (sqlite_stmt);
	
	return (rval == SQLITE_ROW)? 1:0;
}

