/*

	About: Mailer v2.1
	Author: kalixis

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before tr_mailer"
#endif

#if !defined HTTP_ERROR_MALFORMED_RESPONSE
	#error "You must have a_http include in order to use this one."
#endif

#if defined _tr_mailer_included_
	#endinput
#endif
#define _tr_mailer_included_

/*

	Define const

*/

#if !defined TR_MAILER_URL
	#error Please define the URL of the mailer script. Example: #define TR_MAILER_URL "example.com/mailer.php"
#endif

#if !defined TR_EMAIL_ADDRESS
	#error Please define E-Mail address to send messages. Example: #define TR_EMAIL_ADDRESS "mymail@example.com"
#endif

#if !defined MAILER_TYPE_NORMAL
	#define MAILER_TYPE_NORMAL 0
#endif

#if !defined MAILER_TYPE_HTML
	#define MAILER_TYPE_HTML 1
#endif

#if !defined MAX_FUNCTION_NAME
	#define MAX_FUNCTION_NAME 32
#endif

#if !defined MAX_PLAYER_EMAIL
	#define MAX_PLAYER_EMAIL 64
#endif

#if !defined MAX_MAILER_SIZE
	#define MAX_MAILER_SIZE 512
#endif

/*

	Define functions

*/

#define Mail_Create:%0(%1) \
	forward MC_%0(%1); \
	public MC_%0(%1)

#define Mail_Show::%0(%1) \
	MC_%0(%1)

#define Mail_Response:%0(%1) \
	forward MR_%0(%1); \
	public MR_%0(%1)

#define Mail: #

#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

/*

	Forwards

*/

forward OnPlayerMailerScriptResponse(playerid, response_code, const data[]);

/*

	Vars

*/

static
	mail_player_function[MAX_PLAYERS][MAX_FUNCTION_NAME + 1 char];

/*

	Public functions

*/

stock Mail_Send(playerid, const function[], const to[], const additional_headers[], const subject[], const message[], type = MAILER_TYPE_NORMAL)
{
	new
		info[MAX_MAILER_SIZE + 1];

	mail_player_function[playerid] = !"MR_";
	strcat(mail_player_function[playerid], function);
	format(info, sizeof(info), "to=%s&additional_headers=%s&subject=%s&message=%s&additional_parameters="TR_EMAIL_ADDRESS"&type=%i", to, additional_headers, subject, message, type);
	HTTP(playerid, HTTP_POST, TR_MAILER_URL, info, !"OnPlayerMailerScriptResponse");
}

/*

	OnPlayerMailerScriptResponse

*/

public OnPlayerMailerScriptResponse(playerid, response_code, const data[])
{
	if (funcidx(mail_player_function[playerid]) != -1)
		CallLocalFunction(mail_player_function[playerid], !"iis", playerid, response_code, isnull(data) ? !"\1" : data);
	return 1;
}