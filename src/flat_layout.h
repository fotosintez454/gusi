/* flat_layout.h — single source of truth for house / flat geometry.
   All values are in metres, relative to the house origin (hx[h], hz[h]).
   a = x - hx[h] (mirrored per flat side: s=0 -> a<0, s=1 -> a>0), b = z - hz[h].
   Collision (room_solid150), drawing (studio150_draw), lighting (rooms_light160),
   furniture (object150) and NPC sleep paths all read these constants.
   To resize rooms, change values HERE, then run the qa_* tests. */
#ifndef FLAT_LAYOUT_H
#define FLAT_LAYOUT_H

/* House shell */
#define HOUSE_HALF_W      4.8f   /* inner half-width of the house                 */
#define HOUSE_FRONT_B    -4.1f   /* inner face of the street facade (stair hall)  */
#define HOUSE_BACK_B      4.4f   /* inner face of the courtyard facade            */
#define FLOOR_H           3.0f   /* storey height                                 */
#define WALL_T            0.20f  /* drawn partition thickness                     */
#define WALL_HIT          0.10f  /* collision half-thickness of a partition       */

/* Entry wall between stair hall and flats */
#define FLAT_ENTRY_B     -1.9f
#define FLAT_DOOR_A       2.4f   /* |a| of the entry door centre                  */
#define FLAT_DOOR_HALF    0.65f  /* half clear width of the entry door (1.3 m)    */

/* Wet room (bath + WC) in the outer corner of each flat */
#define BATH_A            3.25f  /* |a| of the bath partition                     */
#define BATH_B            0.5f   /* b of the wall closing the bath to the north   */
#define BATH_DOOR_B0     -1.25f  /* bath door opening along the partition         */
#define BATH_DOOR_B1     -0.15f

/* Hall/kitchen  <->  living/bed room partition */
#define ROOM_WALL_B       0.65f
#define ROOM_DOOR_A0      1.95f  /* doorway |a| range: 1.7 m clear (was 1.4 m)    */
#define ROOM_DOOR_A1      2.85f
#define ROOM_DOOR_A     ((ROOM_DOOR_A0+ROOM_DOOR_A1)*.5f)

/* Bedroom furniture anchors (|a|, b) — moved so a 0.8 m path stays free */
#define BED_A             1.1f
#define BED_B             3.3f   /* bed centre; was 3.0 (blocked the doorway)    */
#define BED_HALF_LEN      0.88f
#define DRESSER_A         1.95f  /* was in the middle of the room (2.0, 3.0)      */
#define DRESSER_B         3.95f
#define DESK_CHAIR_B      2.85f

/* v180: real facade apertures; the centre partition never crosses glass. */
#define WINDOW180_W 1.20f
#define WINDOW180_H 1.60f
static float window_a180(int side,int k){return side?(k==0?-3.35f:k==1?-1.10f:2.8f):(-2.8f+k*2.8f);}
static int high_floors180(int n){return n==1||n==3?12:9;}
#endif
