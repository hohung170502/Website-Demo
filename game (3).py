import pygame, sys, random
#Tạo hàm cho trò chơi
def draw_floor():
	#sàn 1 nối tiếp sàn 2 (vị trí kết thúc sàn 1 là vị trí bắt đầu của sàn 2)
    screen.blit(floor,(floor_x_pos,650))
    screen.blit(floor,(floor_x_pos+432,768))
def create_pipe():
	#Tạo thêm 2 ống(trên dưới) với chiều cao ngẫu nhiên có thể là 200px, 300px hoặc 400px
	#giữa hai khoảng trống cần có khoảng hở (đang để 650px)
    random_pipe_pos = random.choice(pipe_height)
    bottom_pipe = pipe_surface.get_rect(midtop =(500,random_pipe_pos))
    top_pipe = pipe_surface.get_rect(midtop =(500,random_pipe_pos-650))
    return bottom_pipe, top_pipe
def move_pipe(pipes):
	#di chuyển ống về bên trái, mỗi lần di chuyển thì lùi lại 5px
    for pipe in pipes :
        pipe.centerx -= 5
    return pipes
def draw_pipe(pipes):
	#Vẽ ống
    for pipe in pipes:
        if pipe.bottom >= 600 :
          screen.blit(pipe_surface,pipe)
        else:
			#Nếu là ống phía trên thì quay ngược đầu lại theo trục y
            flip_pipe = pygame.transform.flip(pipe_surface,False,True)
            screen.blit(flip_pipe,pipe)
def check_collision(pipes):
	#Kiểm tra va chạm
    for pipe in  pipes:
		#Hình chũ nhật chứa con chim giao với hình chữ nhật chứa cái ống => game active = false
        if bird_rect.colliderect(pipe):
			#Nếu chim va chạm thì phát âm thanh va chạm
            hit_sound.play()
            return False
	#Nếu con chim xuống thấp quá, lên cao quá khỏi màn hình game => game active = false
    if bird_rect.top <= -75 or bird_rect.bottom >= 650:
            return False
	#nếu không thì => game active = true
    return True
def rotate_bird(bird1):
	#Xoay chim theo chiều y(hiệu ứng chúi đầu chim lên, xuống)
    new_bird = pygame.transform.rotozoom(bird1,-bird_movement*3,1)
    return new_bird
def bird_animation():
	#Hiệu ứng đập cánh của chim
    new_bird = bird_list[bird_index]
    new_bird_rect = new_bird.get_rect(center = (100,bird_rect.centery))
    return new_bird, new_bird_rect
def score_display(game_state):
	#Hiển thị điểm
	#Khi game vẫn đang active, hiểm thị Score
    if game_state == 'main game':
        score_surface = game_font.render(str(int(score)),True,(255,255,255))
        score_rect = score_surface.get_rect(center = (216,100))
        screen.blit(score_surface,score_rect)
		
	#Khi game kết thúc, hiển thị Score và High score
    if game_state == 'game_over':
        score_surface = game_font.render(f'Score: {int(score)}',True,(255,255,255))
        score_rect = score_surface.get_rect(center = (216,100))
        screen.blit(score_surface,score_rect)

        high_score_surface = game_font.render(f'High Score: {int(high_score)}',True,(255,255,255))
        high_score_rect = high_score_surface.get_rect(center = (216,630))
        screen.blit(high_score_surface,high_score_rect)
def update_score(score,high_score):
	#Cập nhật high score nếu như score > high score hiện tại
    if score > high_score:
        high_score = score
    return high_score
#Load và phát âm thanh
pygame.mixer.pre_init(frequency=44100, size=-16, channels=2, buffer=512)
pygame.init()
#1.Tạo cửa sổ với kích thước cao 432px, rộng 768px 
screen= pygame.display.set_mode((432,768))
clock = pygame.time.Clock()
#Load font chữ từ file có sẵn
game_font = pygame.font.Font('04B_19.ttf',40)
#tạo biến cho trò chơi 
#khi chim bay sẽ chịu bởi 2 lực: trọng lực kéo chim xuống và lực do bấm phím space kéo chim lên
#trọng lực để chim bay, chim càng bay lâu trọng lực càng lớn
gravity = 0.25
#biến tạo sự di chuyển cho chim, lúc đàu chưa di chuyển thì = 0
bird_movement = 0
game_active = True
score =  0
high_score = 0
#3.chèn Background: load hình ảnh từ file và nhân đôi kích thước ảnh gốc
bg = pygame.image.load('assets/background-night.png').convert()
bg = pygame.transform.scale2x(bg)
#4.chèn sàn: load hình ảnh từ file và nhân đôi kích thước ảnh gốc
floor = pygame.image.load('assets/floor.png').convert()
floor = pygame.transform.scale2x(floor)
#4.1. Khởi tạo tọa độ x ban đầu của sàn
floor_x_pos = 0
#5.tạo chim từ 3 hình ảnh tươn ứng với 3 trạng thái đập cánh của con chim
bird_down = pygame.transform.scale2x(pygame.image.load('assets/yellowbird-downflap.png').convert_alpha())
bird_mid = pygame.transform.scale2x(pygame.image.load('assets/yellowbird-midflap.png').convert_alpha())
bird_up = pygame.transform.scale2x(pygame.image.load('assets/yellowbird-upflap.png').convert_alpha())
bird_list= [bird_down,bird_mid,bird_up] #0 1 2
bird_index = 0
bird = bird_list[bird_index]
#bird= pygame.image.load('assets/yellowbird-midflap.png').convert_alpha()
#bird = pygame.transform.scale2x(bird)
#5.1. Tọa độ hình chữ nhật bao quanh con chim, chim sẽ ở giữa hình chữ nhật này, cách trục x là 100px, cách y là 384px (dùng để kiểm tra va chạm)
bird_rect = bird.get_rect(center = (100,384))

#tao timer cho bird 
#7.Event cho con chim: Sau 200ms thì đổi trạng thái cánh chim
birdflap = pygame.USEREVENT + 1
pygame.time.set_timer(birdflap,200)
#6.tạo ống: load hình ảnh từ file và nhân đôi kích thước ảnh gốc
pipe_surface = pygame.image.load('assets/pipe-green.png').convert()
pipe_surface = pygame.transform.scale2x(pipe_surface)
pipe_list =[]
#6.1.tao timer cho ống: Event cho ống: sau 1200ms thì ống mới sẽ xuất hiện
spawnpipe= pygame.USEREVENT
pygame.time.set_timer(spawnpipe, 1200)
pipe_height = [200,300,400]
#Tao man hinh ket thuc
game_over_surface = pygame.transform.scale2x(pygame.image.load('assets/message.png').convert_alpha())
game_over_rect = game_over_surface.get_rect(center=(216,384))
#chen am thanh khi chim đập cánh và va chạm với ống, âm thanh tăng điểm
flap_sound = pygame.mixer.Sound('sound/sfx_wing.wav')
hit_sound = pygame.mixer.Sound('sound/sfx_hit.wav')
score_sound = pygame.mixer.Sound('sound/sfx_point.wav')
score_sound_countdown = 100
#while loop cua tro choi
while True:
	#2.vòng lặp cho sự kiện
    for event in pygame.event.get():
		#2.1.người dùng bấm thoát ra ngoài thì tắt cửa sổ game và thoát hệ thống
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
		#2.2. có bất kì phím nào được bấm xuống
        if event.type == pygame.KEYDOWN:
			#2.2.1. phím space được bấm và chim không chạm ống => hiệu ứng chim nhảy lên xuống + tiếng đập cánh
            if event.key == pygame.K_SPACE and game_active:
                bird_movement = 0
                bird_movement =-11
                flap_sound.play()
			#2.2.2. phím space được bấm và chim chạm ống =>Reset chim, pipe_list... để bắt đầu chơi lại
            if event.key == pygame.K_SPACE and game_active==False:
                game_active = True
                pipe_list.clear()
                bird_rect.center = (100,384)
                bird_movement = 0
                score = 0 
		#2.3. ống mới được xuất hiện sau 1200ms và được thêm vào danh sách ống
        if event.type == spawnpipe:
            pipe_list.extend(create_pipe())
		#2.4. hiệu ứng đập cánh cho chim theo thứ tự down,mid,up
        if event.type == birdflap:
            if bird_index < 2:
                bird_index += 1
            else:
                bird_index =0
            bird, bird_rect = bird_animation()
	#3.1. Hiển thị background bg, bắt đầu từ gốc tọa độ (0,0)
    screen.blit(bg,(0,0))
    if game_active:
         #5.2.chim càng di chuyển thì trọng lực càng tăng
         bird_movement += gravity
		 #5.3.Xoay chim
         rotated_bird = rotate_bird(bird)
		 #5.4.cho chim di chuyển xuống dưới
         bird_rect.centery += bird_movement
         screen.blit(rotated_bird,bird_rect)
		 #6.4. Kiểm tra va chạm
         game_active= check_collision(pipe_list)
         #6.2. Di chuyển tất cả các ống có trong danh sách và trả về danh sách mới
         pipe_list = move_pipe(pipe_list)
		 #6.3. Vẽ mới lại các ống có trong danh sách
         draw_pipe(pipe_list)
		 #Cập nhật điểm
         score += 0.01
		 #Hiển thị điểm hiện tại
         score_display('main game')
		 #Mỗi lần bay và được cộng một điểm thì sẽ có âm thanh tăng điểm
         score_sound_countdown -= 1
         if score_sound_countdown <= 0:
             score_sound.play()
             score_sound_countdown = 100
    else:
		#6.5. Nếu chim va chạm ống thì sẽ hiển thị màn hình kết thúc trò chơi
		#Cập nhật high score
        screen.blit(game_over_surface,game_over_rect)
        high_score = update_score(score,high_score)
        score_display('game_over')
    #4.2. sàn di chuyển về phía bên trái khi chim di chuyển, mỗi lần 1px
    floor_x_pos -= 1
	#4.3. Tạo ra 2 sàn lùi về phía bên trái
    draw_floor()
	#4.4. Nếu chạy hết sàn thứ 2 thì sàn thứ 1 lại chạy lên đầu => sàn luôn chạy liên tục không khoảng hở
    if floor_x_pos <= -432:
        floor_x_pos =0
	#2.2. Hiển thị màn hình
    pygame.display.update()
	#Tăng fps để game mượt hơn
    clock.tick(120)