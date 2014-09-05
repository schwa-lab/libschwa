require "formula"

class Libschwa < Formula
  homepage "https://github.com/schwa-lab/libschwa"
  url "https://github.com/schwa-lab/libschwa/releases/download/0.4.0/libschwa-0.4.0.tar.gz"
  sha1 "b8d4f7796d55c5b2a733506e59382141fb0187f1"

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
