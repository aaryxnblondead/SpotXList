import tkinter as tk
from tkinter import ttk, messagebox
from ctypes import CDLL, c_char_p, c_int

class SpotXGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("SpotX Music Player")
        self.root.geometry("600x400")
        
        # Load C library
        self.lib = CDLL('./spotxlist.so')
        
        # Create main frame
        self.main_frame = ttk.Frame(self.root, padding="10")
        self.main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Now Playing Frame
        self.now_playing_frame = ttk.LabelFrame(self.main_frame, text="Now Playing", padding="5")
        self.now_playing_frame.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        self.now_playing_label = ttk.Label(self.now_playing_frame, text="No track playing")
        self.now_playing_label.grid(row=0, column=0, padx=5)
        
        # Control Buttons
        self.controls_frame = ttk.Frame(self.main_frame)
        self.controls_frame.grid(row=1, column=0, columnspan=2, pady=5)
        
        ttk.Button(self.controls_frame, text="⏮", command=self.previous_track).grid(row=0, column=0, padx=2)
        ttk.Button(self.controls_frame, text="▶", command=self.play_selected).grid(row=0, column=1, padx=2)
        ttk.Button(self.controls_frame, text="⏭", command=self.next_track).grid(row=0, column=2, padx=2)
        
        # Song List
        self.song_frame = ttk.LabelFrame(self.main_frame, text="Library", padding="5")
        self.song_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=5)
        
        self.song_list = tk.Listbox(self.song_frame, height=10)
        self.song_list.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Scrollbar for song list
        scrollbar = ttk.Scrollbar(self.song_frame, orient=tk.VERTICAL, command=self.song_list.yview)
        scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        self.song_list['yscrollcommand'] = scrollbar.set
        
        # Add Song Frame
        self.add_frame = ttk.LabelFrame(self.main_frame, text="Add New Song", padding="5")
        self.add_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        ttk.Label(self.add_frame, text="Title:").grid(row=0, column=0, padx=5)
        self.title_var = tk.StringVar()
        ttk.Entry(self.add_frame, textvariable=self.title_var).grid(row=0, column=1, padx=5)
        
        ttk.Label(self.add_frame, text="Artist:").grid(row=1, column=0, padx=5)
        self.artist_var = tk.StringVar()
        ttk.Entry(self.add_frame, textvariable=self.artist_var).grid(row=1, column=1, padx=5)
        
        ttk.Label(self.add_frame, text="Mood:").grid(row=2, column=0, padx=5)
        self.mood_var = tk.StringVar()
        ttk.Entry(self.add_frame, textvariable=self.mood_var).grid(row=2, column=1, padx=5)
        
        ttk.Label(self.add_frame, text="Duration (s):").grid(row=3, column=0, padx=5)
        self.duration_var = tk.StringVar()
        ttk.Entry(self.add_frame, textvariable=self.duration_var).grid(row=3, column=1, padx=5)
        
        ttk.Button(self.add_frame, text="Add Song", command=self.add_song).grid(row=4, column=0, columnspan=2, pady=5)
        
    def add_song(self):
        try:
            title = self.title_var.get().encode('utf-8')
            artist = self.artist_var.get().encode('utf-8')
            mood = self.mood_var.get().encode('utf-8')
            duration = int(self.duration_var.get())
            
            self.lib.addSong(c_char_p(title), c_char_p(artist), c_char_p(mood), c_int(duration))
            self.refresh_song_list()
            self.clear_inputs()
            messagebox.showinfo("Success", "Song added successfully!")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to add song: {str(e)}")
    
    def play_selected(self):
        selection = self.song_list.curselection()
        if selection:
            title = self.song_list.get(selection[0]).split(" - ")[0].encode('utf-8')
            self.lib.playSong(c_char_p(title))
            self.update_now_playing()
    
    def previous_track(self):
        self.lib.previousSong()
        self.update_now_playing()
    
    def next_track(self):
        self.lib.nextSong()
        self.update_now_playing()
    
    def refresh_song_list(self):
        self.song_list.delete(0, tk.END)
        self.lib.displayAllSongs()
        
    def clear_inputs(self):
        self.title_var.set("")
        self.artist_var.set("")
        self.mood_var.set("")
        self.duration_var.set("")
    
    def update_now_playing(self):
        # This would need modification of C code to return current track info
        pass

if __name__ == "__main__":
    root = tk.Tk()
    app = SpotXGUI(root)
    root.mainloop()
