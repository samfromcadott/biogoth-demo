from PIL import Image
from sys import argv
from os import walk
from glob import glob
from itertools import product

# CHARACTERS = ['guard']
ACTIONS = ['idle', 'walk', 'attack', 'airattack', 'die', 'fall', 'slide', 'bite', 'stun']
DIRECTIONS = ['right', 'left']
# DIRECTIONS = ['right']

CHARACTERS = {
	"guard": {
		"idle": {
			"start": 1,
			"end": 21
		},
		"walk": {
			"start": 22,
			"end": 36
		},
		"attack": {
			"start": 80,
			"end": 89
		},
		"die": {
			"start": 37,
			"end": 79
		},
		"fall": {
			"start": 90,
			"end": 90
		},
		"bite": {
			"start": 91,
			"end": 91
		},
		"stun": {
			"start": 92,
			"end": 92
		}
	}
}

def create_dict():
	# Make the frame dictionary
	frames = {}
	for direction in DIRECTIONS:
		for action in ACTIONS:
			frames[f'{action}-{direction}'] = []

	return frames

def find_frames(character, frames):
	# Look for frames for each action and direction
	searchstring = f'./rendered/{character}[0-9][0-9][0-9][0-9].png'
	files = glob(searchstring)

	for action in CHARACTERS[character]:
		for direction in DIRECTIONS:
			start_frame = CHARACTERS[character][action]["start"]
			end_frame = CHARACTERS[character][action]["end"] + 1
			for frame in range( start_frame, end_frame ):
				frames[f'{action}-{direction}'].append( './rendered/{}-{}{:04d}.png'.format(character, direction, frame) )

def create_canvas(frames):
	# Find the longest action
	anim_length = len( frames['die-right'] ) # length of the animation
	im = Image.open( frames['idle-right'][0] ) # sprite dimmensions
	sprite_width = im.width
	sprite_height = im.height
	del im

	canvas = Image.new( 'RGBA', (anim_length*sprite_width, len(ACTIONS)*sprite_height*2 ), (0, 0, 0, 0) )
	return canvas

def create_sheet(character, canvas, frames):
	# Copy frames onto canvas
	x = 0
	y = 0

	im = Image.open( frames['idle-right'][0] ) # sprite dimmensions
	sprite_width = im.width
	sprite_height = im.height
	del im

	for (direction, action) in product(DIRECTIONS, ACTIONS):
		if action not in CHARACTERS[character]:
			continue

		x = 0
		for frame in frames[ f"{action}-{direction}" ]:
			sprite = Image.open(frame)
			canvas.paste(sprite, (x,y))
			x += sprite_width

		y += sprite_height

for character in CHARACTERS:
	frames = create_dict()
	find_frames(character, frames)
	canvas = create_canvas(frames)
	create_sheet(character, canvas, frames)
	canvas.save(f'{character}.png', 'PNG')
