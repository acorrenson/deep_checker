import pandas
    

data = []
with open("example.csv") as fichier :
    for game in fichier :
        coups = game.split()
        c_couple = []
        for i in range (0,len(coups) - 1,2) :
            couple = (int(coups[i],16),int(coups[i+1],16))
            c_couple.append(couple)
        data.append(c_couple)


    
# Here read the data
