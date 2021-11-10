#ifndef NOTE_PLAYER_GPIO_HPP
#define NOTE_PLAYER_GPIO_HPP

#include "note_player.hpp"
#include "SpeakerController.h"

// ===========================================================================
//
// simple note player for a speaker connected to a GPIO pin
//
// ===========================================================================

class note_player_gpio : public note_player {
private:
	Speaker & lsp;
public:
	note_player_gpio( Speaker & lsp ):
			lsp( lsp )
	{}
	
	void play( const note & n ) override;
};

#endif