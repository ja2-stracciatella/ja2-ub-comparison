#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Intro.h"
	#include "_Ja25EnglishText.h"
#else
	#include "Types.h"
	#include "GameInitOptionsScreen.h"
	#include "GameSettings.h"
	#include "Utilities.h"
	#include "wCheck.h"
	#include "Font Control.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
	#include "Input.h"
	#include "Options Screen.h"
	#include "English.h"
	#include "Sysutil.h"
	#include "Fade Screen.h"
	#include "Cursor Control.h"
	#include "Music Control.h"
	#include "cursors.h"
	#include "Intro.h"
#endif

////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		GIO_TITLE_FONT											FONT16ARIAL//FONT14ARIAL
#define		GIO_TITLE_COLOR											FONT_MCOLOR_WHITE

#define		GIO_TOGGLE_TEXT_FONT								FONT16ARIAL//FONT14ARIAL
#define		GIO_TOGGLE_TEXT_COLOR								FONT_MCOLOR_WHITE

//buttons
#define		GIO_BTN_OK_X												141
#define		GIO_BTN_OK_Y												418
#define		GIO_CANCEL_X												379

//main title
#define		GIO_MAIN_TITLE_X										0
#define		GIO_MAIN_TITLE_Y										68
#define		GIO_MAIN_TITLE_WIDTH								SCREEN_BUFFER_WIDTH

//radio box locations
#define		GIO_GAP_BN_SETTINGS									35
#define		GIO_OFFSET_TO_TEXT									20//30
#define		GIO_OFFSET_TO_TOGGLE_BOX						155//200
#define		GIO_OFFSET_TO_TOGGLE_BOX_Y					9

#define		GIO_DIF_SETTINGS_X									80
#define		GIO_DIF_SETTINGS_Y									150
#define		GIO_DIF_SETTINGS_WIDTH							GIO_OFFSET_TO_TOGGLE_BOX - GIO_OFFSET_TO_TEXT //230

#define		GIO_GAME_SETTINGS_X									350
#define		GIO_GAME_SETTINGS_Y									300//280//150
#define		GIO_GAME_SETTINGS_WIDTH							GIO_DIF_SETTINGS_WIDTH

#define		GIO_GUN_SETTINGS_X									GIO_GAME_SETTINGS_X
#define		GIO_GUN_SETTINGS_Y									GIO_DIF_SETTINGS_Y//150//280
#define		GIO_GUN_SETTINGS_WIDTH							GIO_DIF_SETTINGS_WIDTH

#define		GIO_TIMED_TURN_SETTING_X						GIO_DIF_SETTINGS_X
#define		GIO_TIMED_TURN_SETTING_Y						GIO_GAME_SETTINGS_Y
#define		GIO_TIMED_TURN_SETTING_WIDTH				GIO_DIF_SETTINGS_WIDTH


//Difficulty settings
enum
{
	GIO_DIFF_EASY,
	GIO_DIFF_MED,
	GIO_DIFF_HARD,

	NUM_DIFF_SETTINGS,
};

/*
Ja25 No sci fi options anymore
// Game Settings options
enum
{
	GIO_REALISTIC,
	GIO_SCI_FI,

	NUM_GAME_STYLES,
};

*/

// Iron man mode
enum
{
	GIO_CAN_SAVE,
	GIO_IRON_MAN,

	NUM_SAVE_OPTIONS,
};


// Gun options
enum
{
	GIO_REDUCED_GUNS,
	GIO_GUN_NUT,

	NUM_GUN_OPTIONS,
};

//enum for the timed turns setting
enum
{
	GIO_NO_TIMED_TURNS,
	GIO_TIMED_TURNS,

	GIO_NUM_TIMED_TURN_OPTIONS,
};

//enum for different states of game
enum
{
	GIO_NOTHING,
	GIO_CANCEL,
	GIO_EXIT,
	GIO_IRON_MAN_MODE,
};

////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfGIOScreenEntry = TRUE;
BOOLEAN		gfGIOScreenExit	= FALSE;
BOOLEAN		gfReRenderGIOScreen=TRUE;
BOOLEAN		gfGIOButtonsAllocated = FALSE;

UINT8			gubGameOptionScreenHandler=GIO_NOTHING;

UINT32		gubGIOExitScreen = GAME_INIT_OPTIONS_SCREEN;

UINT32		guiGIOMainBackGroundImage;


INT32			giGioMessageBox = -1;


//BOOLEAN		gfExitGioDueToMessageBox=FALSE;


//UINT8			gubDifficultySettings[ NUM_DIFF_SETTINGS ];
//UINT8			gubGameSettings[ NUM_GAME_STYLES ];
//UINT8			gubGunSettings[ NUM_GUN_OPTIONS ];


// Done Button
void BtnGIODoneCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiGIODoneButton;
INT32		giGIODoneBtnImage;

// Cancel Button
void BtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiGIOCancelButton;
INT32		giGIOCancelBtnImage;


//checkbox to toggle the Diff level
UINT32	guiDifficultySettingsToggles[ NUM_DIFF_SETTINGS ];
void BtnDifficultyTogglesCallback(GUI_BUTTON *btn,INT32 reason);

/*
//checkbox to toggle Game style
UINT32	guiGameStyleToggles[ NUM_GAME_STYLES ];
void BtnGameStyleTogglesCallback(GUI_BUTTON *btn,INT32 reason);
*/
//checkbox to toggle Save style
UINT32	guiGameSaveToggles[ NUM_SAVE_OPTIONS ];
void BtnGameSaveTogglesCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Gun options
UINT32	guiGunOptionToggles[ NUM_GUN_OPTIONS ];
void BtnGunOptionsTogglesCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Timed turn option on or off
UINT32	guiTimedTurnToggles[ GIO_NUM_TIMED_TURN_OPTIONS ];
void BtnTimedTurnsTogglesCallback(GUI_BUTTON *btn,INT32 reason);



////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern		void ClearMainMenu();

BOOLEAN		EnterGIOScreen();
BOOLEAN		ExitGIOScreen();
void			HandleGIOScreen();
BOOLEAN		RenderGIOScreen();
void			GetGIOScreenUserInput();
UINT8			GetCurrentGunButtonSetting();
UINT8			GetCurrentGameSaveButtonSetting();
UINT8			GetCurrentDifficultyButtonSetting();
void			RestoreGIOButtonBackGrounds();
void			DoneFadeOutForExitGameInitOptionScreen( void );
void			DoneFadeInForExitGameInitOptionScreen( void );
UINT8			GetCurrentTimedTurnsButtonSetting();
BOOLEAN		DoGioMessageBox( UINT8 ubStyle, INT16 *zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void			DisplayMessageToUserAboutGameDifficulty();
void			DisplayMessageToUserAboutUserCampaign();
void			ConfirmGioDifSettingMessageBoxCallBack( UINT8 bExitValue );
BOOLEAN		DisplayMessageToUserAboutIronManMode();
void			ConfirmGioIronManMessageBoxCallBack( UINT8 bExitValue );


//ppp




UINT32	GameInitOptionsScreenInit( void )
{
	return( 1 );
}



UINT32	GameInitOptionsScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfGIOScreenEntry )
	{
//		PauseGame();

		EnterGIOScreen();
		gfGIOScreenEntry = FALSE;
		gfGIOScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_BUFFER_WIDTH, SCREEN_BUFFER_HEIGHT );
	}

	GetGIOScreenUserInput();
	

	HandleGIOScreen();

	// render buttons marked dirty	
	MarkButtonsDirty( );
	RenderButtons( ); 

	// render help
//	RenderFastHelp( );
//	RenderButtonsFastHelp( );


	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	if ( HandleFadeOutCallback( ) )
	{
		ClearMainMenu();
		return( gubGIOExitScreen );
	}

	if ( HandleBeginFadeOut( gubGIOExitScreen ) )
	{
		return( gubGIOExitScreen );
	}

	if( gfGIOScreenExit )
	{
		ExitGIOScreen();
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderGIOScreen();
	}

	if ( HandleBeginFadeIn( gubGIOExitScreen ) )
	{
	}

	return( gubGIOExitScreen );
}


UINT32	GameInitOptionsScreenShutdown( void )
{
	return( 1 );
}










BOOLEAN		EnterGIOScreen()
{
  VOBJECT_DESC    VObjectDesc;
	UINT16					cnt;
	UINT16					usPosY;

	if( gfGIOButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	// load the Main trade screen backgroiund image
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("InterFace\\OptionsScreenBackGround.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiGIOMainBackGroundImage ));


	//Ok button
	giGIODoneBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiGIODoneButton = CreateIconAndTextButton( giGIODoneBtnImage, gzGIOScreenText[GIO_OK_TEXT], OPT_BUTTON_FONT, 
													 OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW, 
													 OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW, 
													 TEXT_CJUSTIFIED, 
													 GIO_BTN_OK_X, GIO_BTN_OK_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnGIODoneCallback);

	SpecifyButtonSoundScheme( guiGIODoneButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiGIODoneButton, DISABLED_STYLE_NONE );

	//Cancel button
	giGIOCancelBtnImage = UseLoadedButtonImage( giGIODoneBtnImage, -1,1,-1,3,-1 );
	guiGIOCancelButton = CreateIconAndTextButton( giGIOCancelBtnImage, gzGIOScreenText[GIO_CANCEL_TEXT], OPT_BUTTON_FONT, 
													 OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW, 
													 OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW, 
													 TEXT_CJUSTIFIED, 
													 GIO_CANCEL_X, GIO_BTN_OK_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnGIOCancelCallback );
	SpecifyButtonSoundScheme( guiGIOCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );


	//
	//Check box to toggle Difficulty settings
	//
	usPosY = GIO_DIF_SETTINGS_Y - GIO_OFFSET_TO_TOGGLE_BOX_Y;

	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		guiDifficultySettingsToggles[ cnt ] = CreateCheckBoxButton(	GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10, 
																		BtnDifficultyTogglesCallback );
		MSYS_SetBtnUserData( guiDifficultySettingsToggles[ cnt ], 0, cnt );

		usPosY += GIO_GAP_BN_SETTINGS;
	}
	if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		ButtonList[ guiDifficultySettingsToggles[ GIO_DIFF_EASY ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
		ButtonList[ guiDifficultySettingsToggles[ GIO_DIFF_MED ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
		ButtonList[ guiDifficultySettingsToggles[ GIO_DIFF_HARD ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else
		ButtonList[ guiDifficultySettingsToggles[ GIO_DIFF_MED ] ]->uiFlags |= BUTTON_CLICKED_ON;


	//
	//Check box to toggle Game settings ( realistic, sci fi )
	//

	//Ja25: sci fi is on
	gGameOptions.fSciFi = TRUE;

	usPosY = GIO_GAME_SETTINGS_Y - GIO_OFFSET_TO_TOGGLE_BOX_Y;
	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		guiGameSaveToggles[ cnt ] = CreateCheckBoxButton(	GIO_GAME_SETTINGS_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10, 
																		BtnGameSaveTogglesCallback );
		MSYS_SetBtnUserData( guiGameSaveToggles[ cnt ], 0, cnt );

		usPosY += GIO_GAP_BN_SETTINGS;
	}
	if( gGameOptions.fIronManMode )
		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags |= BUTTON_CLICKED_ON;


//	SpecifyDisabledButtonStyle( guiGameSaveToggles[ GIO_REALISTIC ], DISABLED_STYLE_SHADED );
//	SpecifyDisabledButtonStyle( guiGameSaveToggles[ GIO_SCI_FI ], DISABLED_STYLE_SHADED );

//	DisableButton( guiGameStyleToggles[ GIO_REALISTIC ] );
//	DisableButton( guiGameStyleToggles[ GIO_SCI_FI ] );


	//
	// Check box to toggle Gun options
	//

	usPosY = GIO_GUN_SETTINGS_Y - GIO_OFFSET_TO_TOGGLE_BOX_Y;
	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		guiGunOptionToggles[ cnt ] = CreateCheckBoxButton(	GIO_GUN_SETTINGS_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10, 
																		BtnGunOptionsTogglesCallback);
		MSYS_SetBtnUserData( guiGunOptionToggles[ cnt ], 0, cnt );

		usPosY += GIO_GAP_BN_SETTINGS;
	}


	if( gGameOptions.fGunNut )
		ButtonList[ guiGunOptionToggles[ GIO_GUN_NUT ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiGunOptionToggles[ GIO_REDUCED_GUNS ] ]->uiFlags |= BUTTON_CLICKED_ON;





	//
	// Check box to toggle the timed turn option
	//

	usPosY = GIO_TIMED_TURN_SETTING_Y - GIO_OFFSET_TO_TOGGLE_BOX_Y;
	for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
	{
		guiTimedTurnToggles[ cnt ] = CreateCheckBoxButton(	GIO_TIMED_TURN_SETTING_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10, 
																		BtnTimedTurnsTogglesCallback );
		MSYS_SetBtnUserData( guiTimedTurnToggles[ cnt ], 0, cnt );

		usPosY += GIO_GAP_BN_SETTINGS;
	}
	if( gGameOptions.fTurnTimeLimit )
		ButtonList[ guiTimedTurnToggles[ GIO_TIMED_TURNS ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiTimedTurnToggles[ GIO_NO_TIMED_TURNS ] ]->uiFlags |= BUTTON_CLICKED_ON;




	//Reset the exit screen
	gubGIOExitScreen = GAME_INIT_OPTIONS_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderGIOScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_BUFFER_WIDTH-1, SCREEN_BUFFER_HEIGHT-41 ); //OFFSET: UB uses SCREEN_BUFFER_WIDTH and SCREEN_BUFFER_HEIGHT

	gfGIOButtonsAllocated = TRUE;

	return( TRUE );
}


BOOLEAN		ExitGIOScreen()
{
	UINT16	cnt;

	if( !gfGIOButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiGIOMainBackGroundImage );

	RemoveButton( guiGIOCancelButton );
	RemoveButton( guiGIODoneButton );

	UnloadButtonImage( giGIOCancelBtnImage );
	UnloadButtonImage( giGIODoneBtnImage );


	//Check box to toggle Difficulty settings
	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
		RemoveButton( guiDifficultySettingsToggles[ cnt ] );

	//Check box to toggle Game save settings ( realistic, sci fi )
	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
		RemoveButton( guiGameSaveToggles[ cnt ] );

	//Check box to toggle Gun options
	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
		RemoveButton( guiGunOptionToggles[ cnt ] );


	//remove the timed turns toggle
	for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++ )
		RemoveButton( guiTimedTurnToggles[ cnt ] );

	gfGIOButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	if( gubGameOptionScreenHandler == GIO_EXIT )
		SetMusicMode( MUSIC_NONE );

	gfGIOScreenExit = FALSE;
	gfGIOScreenEntry = TRUE;

	return( TRUE );
}


void			HandleGIOScreen()
{
	if( gubGameOptionScreenHandler != GIO_NOTHING )
	{
		switch( gubGameOptionScreenHandler )
		{
			case GIO_CANCEL:
				gubGIOExitScreen = MAINMENU_SCREEN;
				gfGIOScreenExit	= TRUE;
				break;

			case GIO_EXIT:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitGameInitOptionScreen )
				{
					//Disable the ok button
					DisableButton( guiGIODoneButton );

					gFadeOutDoneCallback = DoneFadeOutForExitGameInitOptionScreen;

					FadeOutNextFrame( );
				}
				break;
			}

			case GIO_IRON_MAN_MODE:
				DisplayMessageToUserAboutGameDifficulty();
				break;
		}

		gubGameOptionScreenHandler = GIO_NOTHING;
	}


	if( gfReRenderGIOScreen )
	{
		RenderGIOScreen();
		gfReRenderGIOScreen = FALSE;
	}

	RestoreGIOButtonBackGrounds();
}


BOOLEAN		RenderGIOScreen()
{
	HVOBJECT	hPixHandle;
	UINT16		usPosY;

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiGIOMainBackGroundImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,0,0, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, 48, 55, 592, 403 );	//358


	//Display the title
	DrawTextToScreen( gzGIOScreenText[ GIO_INITIAL_GAME_SETTINGS ], GIO_MAIN_TITLE_X, GIO_MAIN_TITLE_Y, GIO_MAIN_TITLE_WIDTH, GIO_TITLE_FONT, GIO_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );	



	//Display the Dif Settings Title Text
	//DrawTextToScreen( gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], GIO_DIF_SETTINGS_X, (UINT16)(GIO_DIF_SETTINGS_Y-GIO_GAP_BN_SETTINGS), GIO_DIF_SETTINGS_WIDTH, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	DisplayWrappedString( GIO_DIF_SETTINGS_X, (UINT16)(GIO_DIF_SETTINGS_Y-GIO_GAP_BN_SETTINGS), GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );


	usPosY = GIO_DIF_SETTINGS_Y+2;
	//DrawTextToScreen( gzGIOScreenText[ GIO_EASY_TEXT ], (UINT16)(GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_MAIN_TITLE_WIDTH, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_EASY_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY += GIO_GAP_BN_SETTINGS;
	//DrawTextToScreen( gzGIOScreenText[ GIO_MEDIUM_TEXT ], (UINT16)(GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_MAIN_TITLE_WIDTH, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_MEDIUM_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY += GIO_GAP_BN_SETTINGS;
	//DrawTextToScreen( gzGIOScreenText[ GIO_HARD_TEXT ], (UINT16)(GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_MAIN_TITLE_WIDTH, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_HARD_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );



	//Display the Game Settings Title Text
	DisplayWrappedString( GIO_GAME_SETTINGS_X, (UINT16)(GIO_GAME_SETTINGS_Y-GIO_GAP_BN_SETTINGS), GIO_GAME_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GAME_SAVE_STYLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY = GIO_GAME_SETTINGS_Y+2;
	DisplayWrappedString( (UINT16)(GIO_GAME_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_GAME_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_SAVE_ANYWHERE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY += GIO_GAP_BN_SETTINGS;
	DisplayWrappedString( (UINT16)(GIO_GAME_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_GAME_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_IRON_MAN_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY += 20;
	DisplayWrappedString( (UINT16)(GIO_GAME_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, 220, 2, FONT12ARIAL, GIO_TOGGLE_TEXT_COLOR, zNewTacticalMessages[ TCTL_MSG__CANNOT_SAVE_DURING_COMBAT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// ATE: Campaign text
	usPosY += 25;
	{
		CHAR16 zTempStr[60];

		if ( InDefaultCampaign() )
		{
			swprintf( zTempStr, zNewTacticalMessages[ TCTL_MSG__DEFAULT_CAMPAIGN_LABEL ], GetCurrentCampaign( ) );
		}
		else
		{
			swprintf( zTempStr, zNewTacticalMessages[ TCTL_MSG__CAMPAIGN_LABEL ], GetCurrentCampaign( ) );
		}

		DisplayWrappedString( (UINT16)(GIO_DIF_SETTINGS_X), usPosY, 440, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, zTempStr, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}



	//Display the Gun Settings Title Text
	DisplayWrappedString( GIO_GUN_SETTINGS_X, (UINT16)(GIO_GUN_SETTINGS_Y-GIO_GAP_BN_SETTINGS), GIO_GUN_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GUN_OPTIONS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY = GIO_GUN_SETTINGS_Y+2;
	//DrawTextToScreen( gzGIOScreenText[ GIO_REDUCED_GUNS_TEXT ], (UINT16)(GIO_GUN_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_MAIN_TITLE_WIDTH, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	DisplayWrappedString( (UINT16)(GIO_GUN_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_GUN_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_REDUCED_GUNS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	usPosY += GIO_GAP_BN_SETTINGS;
	DisplayWrappedString( (UINT16)(GIO_GUN_SETTINGS_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_GUN_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GUN_NUT_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );



	//Display the Timed turns Settings Title Text
	DisplayWrappedString( GIO_TIMED_TURN_SETTING_X, (UINT16)(GIO_TIMED_TURN_SETTING_Y-GIO_GAP_BN_SETTINGS), GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TIMED_TURN_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	usPosY = GIO_TIMED_TURN_SETTING_Y+2;

	DisplayWrappedString( (UINT16)(GIO_TIMED_TURN_SETTING_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_NO_TIMED_TURNS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	usPosY += GIO_GAP_BN_SETTINGS;

	DisplayWrappedString( (UINT16)(GIO_TIMED_TURN_SETTING_X+GIO_OFFSET_TO_TEXT), usPosY, GIO_DIF_SETTINGS_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TIMED_TURNS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );



/*
Ja25: changed to iron man
	//Shadow the area around the game settings
	ShadowVideoSurfaceRect( FRAME_BUFFER, 336, 257, 580, 378 );

	//Display the text indicatting that the option is disabled for the expansion pack
	DisplayWrappedString( 440, (UINT16)(GIO_GAME_SETTINGS_Y-GIO_GAP_BN_SETTINGS-4), 150, 2, GIO_TOGGLE_TEXT_FONT, 162, gzGIOScreenText[ GIO_DISABLED_FOR_THE_DEMO_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
*/

	return( TRUE );
}


void			GetGIOScreenUserInput()
{
	InputAtom Event;
//	POINT  MousePos;

//	GetCursorPos(&MousePos);

	while( DequeueEvent( &Event ) )
	{
		if( Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubGameOptionScreenHandler = GIO_CANCEL;
					break;


				case ENTER:
					gubGameOptionScreenHandler = GIO_EXIT;
					break;
			}
		}
	}
}


void BtnDifficultyTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
//		UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			UINT8	cnt;

			for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
			{
				ButtonList[ guiDifficultySettingsToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			UINT8	cnt;
			BOOLEAN fAnyChecked=FALSE;

			//if none of the other boxes are checked, do not uncheck this box
			for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
			{

				if( ButtonList[ guiDifficultySettingsToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}
			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnGameSaveTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
//		UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			UINT8	cnt;

			for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
			{
				ButtonList[ guiGameSaveToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			UINT8	cnt;
			BOOLEAN fAnyChecked=FALSE;

			//if none of the other boxes are checked, do not uncheck this box
			for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
			{
				if( ButtonList[ guiGameSaveToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}
			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}



void BtnGunOptionsTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
//		UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			UINT8	cnt;

			for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
			{
				ButtonList[ guiGunOptionToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			UINT8	cnt;
			BOOLEAN fAnyChecked=FALSE;

			//if none of the other boxes are checked, do not uncheck this box
			for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
			{
				if( ButtonList[ guiGunOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}
			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}



void BtnTimedTurnsTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
//		UINT8	ubButton = (UINT8)MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			UINT8	cnt;

			for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
			{
				ButtonList[ guiTimedTurnToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			UINT8	cnt;
			BOOLEAN fAnyChecked=FALSE;

			//if none of the other boxes are checked, do not uncheck this box
			for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
			{
				if( ButtonList[ guiTimedTurnToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}
			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}




void BtnGIODoneCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//if the user doesnt have IRON MAN mode selected
		if( !DisplayMessageToUserAboutIronManMode() )
		{
			//Confirm the difficulty setting
			if ( !InDefaultCampaign( ) )
			{
				DisplayMessageToUserAboutUserCampaign( );
			}
			else
			{
				DisplayMessageToUserAboutGameDifficulty();
			}
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 


void BtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubGameOptionScreenHandler = GIO_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 


UINT8	GetCurrentDifficultyButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		if( ButtonList[ guiDifficultySettingsToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}

	return( 0 );
}



UINT8	GetCurrentGameSaveButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		if( ButtonList[ guiGameSaveToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}



UINT8	GetCurrentGunButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		if( ButtonList[ guiGunOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentTimedTurnsButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
	{
		if( ButtonList[ guiTimedTurnToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}




void RestoreGIOButtonBackGrounds()
{
	UINT8	cnt;
	UINT16 usPosY;


	usPosY = GIO_DIF_SETTINGS_Y-GIO_OFFSET_TO_TOGGLE_BOX_Y;
	//Check box to toggle Difficulty settings
	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		RestoreExternBackgroundRect( GIO_DIF_SETTINGS_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 34, 29 ); 
		usPosY += GIO_GAP_BN_SETTINGS;
	}

	usPosY = GIO_GAME_SETTINGS_Y-GIO_OFFSET_TO_TOGGLE_BOX_Y;
	//Check box to toggle Game settings ( realistic, sci fi )
	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		RestoreExternBackgroundRect( GIO_GAME_SETTINGS_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 34, 29 ); 

		usPosY += GIO_GAP_BN_SETTINGS;
	}

	usPosY = GIO_GUN_SETTINGS_Y-GIO_OFFSET_TO_TOGGLE_BOX_Y;

	//Check box to toggle Gun options
	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		RestoreExternBackgroundRect( GIO_GUN_SETTINGS_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 34, 29 ); 
		usPosY += GIO_GAP_BN_SETTINGS;
	}


	//Check box to toggle Gun options
	usPosY = GIO_TIMED_TURN_SETTING_Y-GIO_OFFSET_TO_TOGGLE_BOX_Y;
	for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
	{
		RestoreExternBackgroundRect( GIO_TIMED_TURN_SETTING_X+GIO_OFFSET_TO_TOGGLE_BOX, usPosY, 34, 29 ); 
		usPosY += GIO_GAP_BN_SETTINGS;
	}
}


void DoneFadeOutForExitGameInitOptionScreen( void )
{
	// loop through and get the status of all the buttons
	gGameOptions.fGunNut = GetCurrentGunButtonSetting();
	gGameOptions.fIronManMode = GetCurrentGameSaveButtonSetting();
	gGameOptions.ubDifficultyLevel = GetCurrentDifficultyButtonSetting() + 1; 
	gGameOptions.fTurnTimeLimit = GetCurrentTimedTurnsButtonSetting();


//JA25: No longer going to the intro screen
//	gubGIOExitScreen = INTRO_SCREEN;

	//Goto the mapscren
	gubGIOExitScreen = INIT_SCREEN;

	//set the fact that we should do the intro videos
/*
Ja25: No intro videos
		SetIntroType( INTRO_BEGINING );
*/
	ExitGIOScreen();

//	gFadeInDoneCallback = DoneFadeInForExitGameInitOptionScreen;
//	FadeInNextFrame( );
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitGameInitOptionScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}


BOOLEAN DoGioMessageBox( UINT8 ubStyle, INT16 *zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
  SGPRect CenteringRect= {0, 0, SCREEN_BUFFER_WIDTH-1, SCREEN_BUFFER_HEIGHT-1 };
  
	// reset exit mode
//	gfExitGioDueToMessageBox = TRUE;

	// do message box and return
  giGioMessageBox = DoMessageBox(  ubStyle,  zString,  uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),  ReturnCallback,  &CenteringRect );

	// send back return state
	return( ( giGioMessageBox != -1 ) );
}


void	PressReleaseMessageBoxCallBack( UINT8 bExitValue )
{
	//if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_EXIT;
	}
}


void DisplayMessageToUserAboutPressPreview()
{
	DoGioMessageBox( MSG_BOX_BASIC_STYLE, gzNewLaptopMessages[LPTP_MSG__PREVIEW_TEXT], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_OK, PressReleaseMessageBoxCallBack );
}


void DisplayMessageToUserAboutGameDifficulty()
{

	UINT8 ubDiffLevel = GetCurrentDifficultyButtonSetting();

	switch( ubDiffLevel )
	{
		case 0:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_NOVICE], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 1:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_EXPERIENCED], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 2:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_EXPERT], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
	}
}

void DisplayMessageToUserAboutUserCampaign()
{
	UINT16 zTempStr[500];

	swprintf( zTempStr, zNewTacticalMessages[ TCTL_MSG__NEW_CAMPAIGN_CONFIRM ], GetCurrentCampaign( ), GetCurrentCampaign( ) );

	DoGioMessageBox( MSG_BOX_BASIC_STYLE, zTempStr, GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
}


void	ConfirmGioDifSettingMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_EXIT;
	}
}

BOOLEAN DisplayMessageToUserAboutIronManMode()
{
	UINT8 ubIronManMode = GetCurrentGameSaveButtonSetting();

	//if the user has selected IRON MAN mode
	if( ubIronManMode )
	{
		DoGioMessageBox( MSG_BOX_BASIC_STYLE, gzIronManModeWarningText[ IMM__IRON_MAN_MODE_WARNING_TEXT ], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioIronManMessageBoxCallBack );

		return( TRUE );
	}

	return( FALSE );
}

void			ConfirmGioIronManMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_IRON_MAN_MODE;
	}
	else
	{
		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}
