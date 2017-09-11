#!/usr/bin/ruby

if(ARGV.size != 3)
  $stderr.puts "Usage: make_card3cx.rb angl.dat wave.dat fnam.dat"
  exit
end

angls = ARGV[0]
waves = ARGV[1]
fnams = ARGV[2]

lines = (File.readlines(angls) rescue -1)
if(lines == -1)
  $stderr.puts "Error, cannot open #{angls}"
  exit
end
angl = []
lines.each{|line|
  item = line.chomp.split
  if(item.size > 1)
    val = item[1]
  else
    val = item[0]
  end
  if(/[^\d\.eE+-]/ =~ val)
    $stderr.puts "Read error >>> #{line}"
    exit
  end
  angl << val.to_f
}
NANG = angl.size

lines = (File.readlines(waves) rescue -1)
if(lines == -1)
  $stderr.puts "Error, cannot open #{waves}"
  exit
end
wave = []
lines.each{|line|
  item = line.chomp.split
  if(item.size > 1)
    val = item[1]
  else
    val = item[0]
  end
  if(/[^\d\.eE+-]/ =~ val)
    $stderr.puts "Read error >>> #{line}"
    exit
  end
  wave << val.to_f
}
NWLF = wave.size

lines = (File.readlines(fnams) rescue -1)
if(lines == -1)
  $stderr.puts "Error, cannot open #{fnams}"
  exit
end
NFUN = lines.size
if(NFUN != NWLF)
  $stderr.puts "Error, NWLF=#{NWLF}, NFUN=#{NFUN}"
  exit
end
fnam = []
0.upto(3){|i|
  fnam[i] = []
}
lines.each{|line|
  item = line.split
  if(item.size < 4)
    $stderr.puts "Error, expected 4 file names >>> #{line}"
    exit
  end
  0.upto(3){|i|
    val = item[i]
    if(!File.exist?(val))
      $stderr.puts "Error, no such file >>> #{line}"
      exit
    end
    fnam[i] << val
  }
}

# CARD3B1
printf("%5d%5d\n",NANG,NWLF)

# CARD3C1
i = 0
angl.each{|val|
  printf(" %9.3f",val)
  i += 1
  if(i%8 == 0)
    printf("\n")
  end
}
if(i%8 != 0)
  printf("\n")
end

# CARD3C2
i = 0
wave.each{|val|
  printf(" %9.3f",val)
  i += 1
  if(i%8 == 0)
    printf("\n")
  end
}
if(i%8 != 0)
  printf("\n")
end

# CARD3C3-3C6
0.upto(3){|n|
  pfun = []
  0.upto(NANG){|i|
    pfun[i] = []
  }
  fnam[n].each{|val|
    if(!File.exist?(val))
      $stderr.puts "Error, no such file >>> #{val}"
      exit
    end
    lines = File.readlines(val)
    if(lines.size != NANG)
      $stderr.puts "Error, NANG=#{NANG}, #{val} size=#{lines.size}"
      exit
    end
    0.upto(NANG-1){|i|
      line = lines[i]
      item = line.chomp.split
      if(item.size > 1)
        v = item[1]
      else
        v = item[0]
      end
      if(/[^\d\.eE+-]/ =~ v)
        $stderr.puts "Read error >>> #{line}"
        exit
      end
      pfun[i] << v.to_f
    }
  }
  0.upto(NANG-1){|i|
    j = 0
    pfun[i].each{|val|
      printf(" %9.3f",val)
      j += 1
      if(j%8 == 0)
        printf("\n")
      end
    }
    if(j%8 != 0)
      printf("\n")
    end
  }
}
