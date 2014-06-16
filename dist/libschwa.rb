require "formula"

class Libschwa < Formula
  homepage "https://github.com/schwa-lab/libschwa"
  url "https://github.com/schwa-lab/libschwa/releases/download/0.3.0/libschwa-0.3.0.tar.gz"
  sha1 "4d03883d2318352856c0a2dd6bc2bbf4bab6c202"

  depends_on "pkg-config"
  depends_on "zeromq" => :optional

  def install
    system "./configure", "--prefix=#{prefix}"
    system "make"
    system "make", "check"
    system "make", "install"
  end

  test do
    system "#{bin}/schwa-tokenizer", "--version"
  end
end
