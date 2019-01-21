clc
clear all
img = double(imread('d:\font16.png'));
in = 1;
for row = 0:3
    for z = 0:79
            for x = 1:8
            	fn(:, x) = img(1+row*16:16+row*16, x+8*z, 1);
            end
            fnt(:,:,in) = fn(:, :);
            in = in + 1;
    end
end
%%
in = 1;
for z = 1:256
    for y = 1:16
        font(in) = uint8(bin2dec(num2str(fnt(y,1:8,z))));
        in = in + 1;
    end
end
f = fopen('font16.bin', 'w');
fwrite(f, font, 'uint8');
fclose(f);
imagesc(fnt(:,:,2));
axis square;
%%
clc
clear all
img = double(imread('d:\font8.png'));
in = 1;
for row = 0:3
    for z = 0:79
            for x = 1:8
            	fn(:, x) = img(1+row*8:8+row*8, x+8*z, 1);
            end
            fnt(:,:,in) = fn(:, :);
            in = in + 1;
    end
end
%%
in = 1;
for z = 1:256
    for y = 1:8
        font(in) = uint8(bin2dec(num2str(fnt(y,1:8,z))));
        in = in + 1;
    end
end
f = fopen('font8.bin', 'w');
fwrite(f, font, 'uint8');
fclose(f);
imagesc(fnt(:,:,2));
axis square;