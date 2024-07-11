#include <linux/ctype.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/string.h>

#include "encdec.h"

#define MODULE_NAME "encdec"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YOUR NAME");

int 	encdec_open(struct inode *inode, struct file *filp);
int 	encdec_release(struct inode *inode, struct file *filp);
int 	encdec_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

ssize_t encdec_read_caesar( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t encdec_write_caesar(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

ssize_t encdec_read_xor( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t encdec_write_xor(struct file *filp, const char *buf, size_t count, loff_t *f_pos);


MODULE_PARM(memory_size, "i");

int major = 0;

///////////////////////////////I ADDED/////////////////////////////////////////////

void cesarEncrypt(char* str, char key);

void cesarDecrypt(char* str, char key);

void xor_Encrypt_Decryptvoid(char* str, char key);

ssize_t encdec_write(int cesarORxor, struct file* filp, const char* buf, size_t count, loff_t* f_pos);
ssize_t encdec_read(int cesarORxor, struct file* filp, char* buf, size_t count, loff_t* f_pos);
//the size of the splitters
int memory_size = 0;

//THE SPLITTERS
char* xorBuffer;
char* cesarBuffer;



///////open,release and ioctl are the same for both caesar and xor/////
struct file_operations fops_caesar = {
	.open 	 =	encdec_open,
	.release =	encdec_release,
	.read 	 =	encdec_read_caesar,
	.write 	 =	encdec_write_caesar,
	.llseek  =	NULL,
	.ioctl 	 =  encdec_ioctl,
	.owner 	 =	THIS_MODULE
};

struct file_operations fops_xor = {
	.open 	 =	encdec_open,
	.release =	encdec_release,
	.read 	 =	encdec_read_xor,
	.write 	 =	encdec_write_xor,
	.llseek  =	NULL,
	.ioctl 	 =  encdec_ioctl,
	.owner 	 =	THIS_MODULE
};

// Implemetation suggestion:
// -------------------------
// Use this structure as your file-object's private data structure
typedef struct {
	unsigned char key;
	int read_state;
} encdec_private_date;



int init_module(void)
{

	// Implemetation suggestion:
	// -------------------------
	//1.Register the device driver and associate it with a Major number using the chrdev_r call

	major = register_chrdev(major, MODULE_NAME, &fops_caesar);
	if(major < 0)
	{
		return major;
	}


	// 2. Allocate memory for the two device buffers using kmalloc (each of them should be of size 'memory_size')

	//The GFP_KERNEL flag indicates that the allocation is being made from the kernel

	xorBuffer = (char*)kmalloc(memory_size, GFP_KERNEL);
    cesarBuffer=(char*)kmalloc(memory_size,GFP_KERNEL);

    if(!xorBuffer||!cesarBuffer)
         return -1;

	return 0;
}

void cleanup_module(void)
{
	// Implemetation suggestion:
	// -------------------------
	// 1. Unregister the device-driver
	unregister_chrdev(major, MODULE_NAME);


	// 2. Free the allocated device buffers using kfree
	kfree(cesarBuffer);
	kfree(xorBuffer);
}

int encdec_open(struct inode *inode, struct file *filp)
{
    if(!inode||!filp)
    {
        return -1;
    }


	int minor = MINOR(inode->i_rdev);

	// Implemetation suggestion:
	// -------------------------
	// 1. Set 'filp->f_op' to the correct file-operations structure (use the minor value to determine which)

	if(minor==0) ///cesar
    {
		filp->f_op = &fops_caesar;
    }
    else{
        if(minor==1)///xor
        {
            filp->f_op = &fops_xor;
        }else///another device
        {
            return -1;
        }
    }


	// 2. Allocate memory for 'filp->private_data' as needed (using kmalloc)
	encdec_private_date* private_data;

	if(!(private_data = (encdec_private_date*)kmalloc(sizeof(encdec_private_date),GFP_KERNEL)))
        return -1;


    private_data->key=0;

    private_data->read_state=ENCDEC_READ_STATE_DECRYPT;

    filp->private_data=(void*)private_data;

	return 0;
}

int encdec_release(struct inode *inode, struct file *filp)
{
	// Implemetation suggestion:
	// -------------------------
	// 1. Free the allocated memory for 'filp->private_data' (using kfree)

	if (!inode || !filp)
		return -1;

	kfree(filp->private_data);
	return 0;
}

int encdec_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	// Implemetation suggestion:
	// -------------------------
	// Update the relevant fields in 'filp->private_data' according to the values of 'cmd' and 'arg'

	if (inode && filp)
	{
		int minor = MINOR(inode->i_rdev), i = 0;
		char* buffer_to_clear = cesarBuffer;
		encdec_private_date* private_data = (encdec_private_date*)filp->private_data;

		switch (cmd) {

		case ENCDEC_CMD_CHANGE_KEY:
			private_data->key = arg;
			return 0;

		case ENCDEC_CMD_SET_READ_STATE:
			private_data->read_state = arg;
			return 0;

		case ENCDEC_CMD_ZERO:

			if (minor != 1 && minor != 0)
				return -1;

			if (minor == 0)
			{
				buffer_to_clear = cesarBuffer;
			}
			if (minor == 1)
			{
				buffer_to_clear = xorBuffer;
			}
			for (i = 0; i < memory_size; i++)
			{
				buffer_to_clear[i] = 0;
			}
			return 0;
		}
	}

	return -1;
}

// Add implementations for:
// ------------------------


void cesarEncrypt(char* str, char key)
{
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		str[i] = (str[i] + key) % 128;
	}
}

void cesarDecrypt(char* str, char key)
{
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		str[i] = (str[i] - key +128 ) % 128;
	}
}

void xor_Encrypt_Decryptvoid(char* str, char key)
{
	int i;
	for ( i = 0; i < strlen(str); i++)
	{
		str[i] = str[i] ^ key;
	}
}

ssize_t encdec_read(int cesarORxor, struct file* filp, char* buf, size_t count, loff_t* f_pos)
{
	if (!filp || !buf)
		return -1;

	/// we can't read from Buffer then return -EINVAL error
	if (*f_pos >= memory_size)
		return -EINVAL;


	///we can read from Buffer


	encdec_private_date* private_data = (encdec_private_date*)filp->private_data;

	int from_where_to_read = *f_pos, where_to_write = 0,buffer_remaining_to_read;

	if (cesarORxor == 1)
	{
		buffer_remaining_to_read = strlen(cesarBuffer + from_where_to_read);

	}
	else {
		buffer_remaining_to_read = strlen(xorBuffer + from_where_to_read);
	}

	if (count < buffer_remaining_to_read)
		buffer_remaining_to_read = count;

	char* kernel_temp_buffer;

	// Allocate kernel buffer
	if (!(kernel_temp_buffer = (char*)kmalloc(buffer_remaining_to_read, GFP_KERNEL)))
		return -1;

	char* temp;

	if (cesarORxor == 1)
	{
		temp = cesarBuffer;

	}
	else {
		temp = xorBuffer;
	}

	// read the encrypted data from the Cipher device

	while (from_where_to_read < memory_size && where_to_write < buffer_remaining_to_read)
	{
		kernel_temp_buffer[where_to_write] = temp[from_where_to_read];
		from_where_to_read++;
		where_to_write++;

	}


	// Perform Caesar or xor Cipher decryption on the kernel buffer
	if (private_data->read_state == ENCDEC_READ_STATE_DECRYPT)
	{
		if (cesarORxor == 1)
			cesarDecrypt(kernel_temp_buffer, private_data->key);
		else
			xor_Encrypt_Decryptvoid(kernel_temp_buffer, private_data->key);
	}

	// Copy data to user space from kernel space
	copy_to_user(buf, kernel_temp_buffer, where_to_write);

	// Update the file position for the next read operation
	*f_pos += where_to_write;

	// Free the kernel buffer
	kfree(kernel_temp_buffer);

	return where_to_write;

}

ssize_t encdec_write(int cesarORxor, struct file* filp, const char* buf, size_t count, loff_t* f_pos)
{
	if (!filp || !buf)
		return -1;

	/// we can't write to Buffer then return -ENOSPC error
	if (*f_pos >= memory_size)
		return -ENOSPC;


	///we can write to Buffer

	encdec_private_date* private_data = (encdec_private_date*)filp->private_data;

	int where_to_write = *f_pos, from_where_to_read = 0, user_buf_size = strlen(buf);


	if (count < user_buf_size)
		user_buf_size = count;

	char* kernel_temp_buffer;

	// Allocate kernel buffer
	if (!(kernel_temp_buffer = (char*)kmalloc(user_buf_size, GFP_KERNEL)))
		return -1;


	// Copy data from user space to kernel space
	copy_from_user(kernel_temp_buffer, buf, user_buf_size);

	char* temp;

	// Perform Caesar or xor Cipher encryption on the kernel buffer
	if (cesarORxor == 1)
	{
		cesarEncrypt(kernel_temp_buffer, private_data->key);
		temp = cesarBuffer;

	}
	else {
		xor_Encrypt_Decryptvoid(kernel_temp_buffer, private_data->key);
		temp = xorBuffer;
	}


	// Write the encrypted data to the Cipher device
	while (where_to_write < memory_size && from_where_to_read < user_buf_size)
	{
		temp[where_to_write] = kernel_temp_buffer[from_where_to_read];
		from_where_to_read++;
		where_to_write++;

	}

	// Update the file position for the next write operation
	*f_pos += from_where_to_read;

	// Free the kernel buffer
	kfree(kernel_temp_buffer);

	return  from_where_to_read;

}



// 1. ssize_t encdec_read_caesar( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t encdec_read_caesar(struct file* filp, char* buf, size_t count, loff_t* f_pos)
{

	return encdec_read(1, filp, buf, count, f_pos);

}



// 2. ssize_t encdec_write_caesar(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
ssize_t encdec_write_caesar(struct file* filp, const char* buf, size_t count, loff_t* f_pos)
{

	return encdec_write(1, filp, buf, count, f_pos);

}


// 3. ssize_t encdec_read_xor( struct file *filp, char *buf, size_t count, loff_t *f_pos );
ssize_t encdec_read_xor(struct file* filp, char* buf, size_t count, loff_t* f_pos)
{
	return encdec_read(0, filp, buf, count, f_pos);

}

// 4. ssize_t encdec_write_xor(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
ssize_t encdec_write_xor(struct file* filp, const char* buf, size_t count, loff_t* f_pos)
{
	return encdec_write(0, filp, buf, count, f_pos);
}
