import pygame
import os

# initializing
pygame.init()

# screen size
screen_width = 640
screen_height = 480
screen = pygame.display.set_mode((screen_width, screen_height))

# display title
pygame.display.set_caption("GAME 1")

# FPS
clock = pygame.time.Clock()

# initializing setting
current_path = os.path.dirname(__file__) # gives the location of the file
image_path = os.path.join(current_path, "images") # images folder location

# background
background = pygame.image.load(os.path.join(image_path, "background.png"))

# stage
stage = pygame.image.load(os.path.join(image_path, "stage.png"))
stage_size = stage.get_rect().size
stage_height = stage_size[1]

# character
character = pygame.image.load(os.path.join(image_path, "character.png"))
character_size = character.get_rect().size
character_width = character_size[0]
character_height = character_size[1]
character_x_pos = screen_width/2 - character_width/2
character_y_pos = screen_height - (character_height + stage_height)

# character movement
character_to_x_LEFT = 0
character_to_x_RIGHT = 0

# character speed
character_speed = 10

# weapon
weapon = pygame.image.load(os.path.join(image_path, "weapon.png"))
weapon_size = weapon.get_rect().size
weapon_width = character_size[0]

# firing weapons multiple times
weapons = []

# weapon speed
weapon_speed = 10

# ball
ball_images = [
    pygame.image.load(os.path.join(image_path, "ballon1.png")),
    pygame.image.load(os.path.join(image_path, "ballon2.png")),
    pygame.image.load(os.path.join(image_path, "ballon3.png")),
    pygame.image.load(os.path.join(image_path, "ballon4.png"))
]

# ball initial speed
ball_speed_y = [-18, -15, -12, -9]

# balls
balls = []

balls.append({
    "pos_x": 50, # ball's initial x pos
    "pos_y": 50, # ball's initial y pos
    "img_idx": 0, # ball image's index
    "to_x": 3, # ball's x direction
    "to_y": -6, # ball's y direction
    "init_spd_y": ball_speed_y[0] # ball's initial y speed
})

# used weapon and ball after collision
weapon_to_remove = -1
ball_to_remove = -1

# font setting
game_font = pygame.font.Font(None, 40)
total_time = 40
start_ticks = pygame.time.get_ticks()

# game over message
game_result = "GAME OVER"

# event loop
running = True
while running:
    dt = clock.tick(30) # determines FPS

    for event in pygame.event.get(): # checking for event/action
        if event.type == pygame.QUIT: # checking if game tab is closed
            running = False # game stopped

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                character_to_x_LEFT -= character_speed
            elif event.key == pygame.K_RIGHT:
                character_to_x_RIGHT += character_speed
            elif event.key == pygame.K_SPACE:
                weapon_x_pos = character_x_pos + character_width/2 - weapon_width/2
                weapon_y_pos = character_y_pos
                weapons.append([weapon_x_pos, weapon_y_pos])


        if event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT:
                character_to_x_LEFT = 0
            if event.key == pygame.K_RIGHT:
                character_to_x_RIGHT = 0

    # setting character movement on screen
    character_x_pos += character_to_x_RIGHT + character_to_x_LEFT

    if character_x_pos <= 0:
        character_x_pos = 0
    elif character_x_pos >= screen_width - character_width:
        character_x_pos = screen_width - character_width

    # weapon position
    weapons = [[w[0], w[1] - weapon_speed] for w in weapons] # moving weapon up

    # deleting weapon after hitting screen top
    weapons = [[w[0], w[1]] for w in weapons if w[1] > 0]

    # ball location
    for ball_idx, ball_val in enumerate(balls):
        ball_pos_x  = ball_val["pos_x"]
        ball_pos_y = ball_val["pos_y"]
        ball_img_idx = ball_val["img_idx"]

        ball_size = ball_images[ball_img_idx].get_rect().size
        ball_width = ball_size[0]
        ball_height = ball_size[1]

        # changing direction upon hitting the wall (bouncing effect)
        if ball_pos_x <= 0 or ball_pos_x > screen_width - ball_width:
            ball_val["to_x"] = ball_val["to_x"] * -1

        # vertical position
        if ball_pos_y >= screen_height - stage_height - ball_height:
            ball_val["to_y"] = ball_val["init_spd_y"]
        else:
            ball_val["to_y"] += 0.5

        ball_val["pos_x"] += ball_val["to_x"]
        ball_val["pos_y"] += ball_val["to_y"]

    # character rect info update
    character_rect = character.get_rect()
    character_rect.left = character_x_pos
    character_rect.top = character_y_pos

    for ball_idx, ball_val in enumerate(balls):
        ball_pos_x = ball_val["pos_x"]
        ball_pos_y = ball_val["pos_y"]
        ball_img_idx = ball_val["img_idx"]

        # ball rect info update
        ball_rect = ball_images[ball_img_idx].get_rect()
        ball_rect.left = ball_pos_x
        ball_rect.top = ball_pos_y

        # ball and character collision
        if character_rect.colliderect(ball_rect):
            running = False
            break

        # ball and weapon collision
        for weapon_idx, weapon_val in enumerate(weapons):
            weapon_pos_x = weapon_val[0]
            weapon_pos_y = weapon_val[1]

            # weapon rect info update
            weapon_rect = weapon.get_rect()
            weapon_rect.left = weapon_pos_x
            weapon_rect.top = weapon_pos_y

            # collision check
            if weapon_rect.colliderect(ball_rect):
                weapon_to_remove = weapon_idx
                ball_to_remove = ball_idx

                # dividing ball if it's not the smallest ball size
                if ball_img_idx < 3:
                    # current ball size info
                    ball_width = ball_rect.size[0]
                    ball_height = ball_rect.size[1]

                    # divided ball info
                    small_ball_rect = ball_images[ball_img_idx + 1].get_rect()
                    small_ball_width = small_ball_rect.size[0]
                    small_ball_height = small_ball_rect.size[1]

                    # balls bouncing on the left
                    balls.append({
                        "pos_x": ball_pos_x + ball_width/2 - small_ball_width/2,  # ball's initial x pos
                        "pos_y": ball_pos_y + ball_height/2 - small_ball_height/2,  # ball's initial y pos
                        "img_idx": ball_img_idx + 1,  # ball image's index
                        "to_x": -3,  # ball's x direction
                        "to_y": -6,  # ball's y direction
                        "init_spd_y": ball_speed_y[ball_img_idx + 1]  # ball's initial y speed
                    })

                    # balls bouncing on the right
                    balls.append({
                        "pos_x": ball_pos_x + ball_width / 2 - small_ball_width / 2,  # ball's initial x pos
                        "pos_y": ball_pos_y + ball_height / 2 - small_ball_height / 2,  # ball's initial y pos
                        "img_idx": ball_img_idx + 1,  # ball image's index
                        "to_x": 3,  # ball's x direction
                        "to_y": -6,  # ball's y direction
                        "init_spd_y": ball_speed_y[ball_img_idx + 1]  # ball's initial y speed
                    })
                break
        else:
            continue
        break

    if ball_to_remove > -1:
        del balls[ball_to_remove]
        ball_to_remove = -1

    if weapon_to_remove > -1:
        del weapons[weapon_to_remove]
        weapon_to_remove = -1

    if len(balls) == 0:
        game_result = "Mission Success"
        running = False

    screen.blit(background, (0, 0))

    for weapon_x_pos, weapon_y_pos in weapons:
        screen.blit(weapon, (weapon_x_pos, weapon_y_pos))

    for idx, val in enumerate(balls):
        ball_pos_x = val["pos_x"]
        ball_pos_y = val["pos_y"]
        ball_img_idx = val["img_idx"]
        screen.blit(ball_images[ball_img_idx], (ball_pos_x, ball_pos_y))

    screen.blit(stage, (0, screen_height - stage_height))
    screen.blit(character, (character_x_pos, character_y_pos))

    # elapsed time
    elapsed_time = (pygame.time.get_ticks() - start_ticks) / 1000  # ms -> s
    timer = game_font.render("Time : {}".format(int(total_time - elapsed_time)), True, (255, 255, 255))
    screen.blit(timer, (10, 10))

    # time over
    if total_time - elapsed_time <= 0:
        game_result = "Time Over"
        running = False

    pygame.display.update()

# game over message
msg = game_font.render(game_result, True, (255,255,0))
msg_rect = msg.get_rect(center=(int(screen_width/2),int(screen_height/2)))
screen.blit(msg, msg_rect)

pygame.display.update()

# 2 sec delay
pygame.time.delay(2000)

pygame.quit()


