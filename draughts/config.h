#ifndef CONFIG_H
#define CONFIG_H

#define CHESS_PIECE_SIZE 50
#define ANIMATION_TIME 400

// MOVE <src_x> <src_y> <dest_x> <dest_y>
#define OPER_MOVE   "MOVE"
// GIVEUP
#define OPER_GIVEUP "GIVEUP"
// START
#define OPER_START "START"
// SETGAME <first_player [B|W]> <game_status>
#define OPER_SETGAME "SETGAME"
// MAKEPEACE
#define OPER_MAKEPEACE "MAKEPEACE"
// CONFIRM_PEACE
#define OPER_CONFIRMPEACE "CONFIRM_PEACE"
// DENIED_PEACE
#define OPER_DENIEDPEACE "DENIED_PEACE"

#define OPER_SETGAME_BLACK 'b'
#define OPER_SETGAME_BKING 'B'
#define OPER_SETGAME_WHITE 'w'
#define OPER_SETGAME_WKING 'W'

#endif
