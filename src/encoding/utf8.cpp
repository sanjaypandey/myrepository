#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <istream>
#include <fstream>
#include <sstream>

using namespace std;

ifstream in;
typedef std::string StringType;

inline unsigned int GetBytesCount(unsigned char leadingByte)
		{
			// 0 means invalid leading byte.
			static const unsigned char BytesCount[256] =
			{
				0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
				3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
				4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			};
			return BytesCount[leadingByte];
		}

inline int ReadCharacter(int &result)
{
            int leadingByte;
			if (in!=NULL&&((leadingByte=in.get())!=std::char_traits<char>::eof()))
			{
				// Try to read the first code unit (leading byte).
				// = ;
				//printf("c:%c\n",leadingByte);

				// If not failbit.
				if (leadingByte != std::char_traits<char>::eof())
				{
					unsigned int bytesCount = GetBytesCount(static_cast<unsigned char>(leadingByte));

					if (bytesCount == 1)
					{
						result = static_cast<int>(static_cast<unsigned char>(leadingByte));
						return 1; // The character was read successfully.
					}

					if (bytesCount != 0) // 2, 3 or 4.
					{
					    //printf("\n############################\n");
					    //printf("\n******byte count:%d ********\n",bytesCount);
						result = 0;
						int continuationByte = 0;
						int codeUnit;
						for (unsigned int i = 1; i < bytesCount; ++i)
						{
							codeUnit = in.get();

							// If failbit...
							if (codeUnit == std::char_traits<char>::eof())
							{
								// Because of the end of the stream.
								if ((in.rdstate() & std::istream::eofbit) != 0)
								{
									// End of the stream, but the leading byte told us
									// there should be the continuation byte, so we treat
									// this like invalid character.
									result = static_cast<int>(static_cast<unsigned char>(leadingByte));
									return -1; // Invalid character.
								}
								return -2; // Something terrible with the stream.
							}

							if ((codeUnit & 0xC0) != 0x80)
							{
								// Two most significant bits in continuation byte should be 1 followed by 0.
								result = static_cast<int>(static_cast<unsigned char>(leadingByte));
								return -1; // Invalid character.
							}

							continuationByte = static_cast<int>(static_cast<unsigned char>(codeUnit));
							continuationByte &= 0x3F; // Hide 2 most significant bits.
							continuationByte <<= ((bytesCount - i - 1) * 6);
							result |= continuationByte;
						}

						if (bytesCount == 2)
						{
							continuationByte = static_cast<int>(static_cast<unsigned char>(leadingByte));
							continuationByte = (continuationByte & 0x1F) << 6;
							result |= continuationByte;
							if (result > 0x7F)
								return 1;
							return -1;
						}

						if (bytesCount == 3)
						{
							continuationByte = static_cast<int>(static_cast<unsigned char>(leadingByte));
							continuationByte = (continuationByte & 0x0F) << 12;
							result |= continuationByte;
							//printf("\n##### result:%x ######\n",result);
							if ((result > 0x07FF && result < 0xD800) ||
								(result > 0xDFFF && result <= 0xFFFD))
								return 1;
							return -1;
						}

						// bytesCount == 4.
						continuationByte = static_cast<int>(static_cast<unsigned char>(leadingByte));
						continuationByte = (continuationByte & 0x07) << 18;
						result |= continuationByte;
						if (result > 0xFFFF && result <= 0x10FFFF)
							return 1;
						return -1;
					}

					result = static_cast<int>(static_cast<unsigned char>(leadingByte));
					return -1; // Invalid character.
				}
				else if ((in.rdstate() & std::istream::eofbit) != 0)
				{
					return 0; // End of the stream.
				}
			}
			return -2; // Something terrible with the stream.
}

inline void WriteCharacter(StringType& stringDestination, int codePoint)
{
			if (codePoint <= 0x7F)
			{
                //printf("\none char is readed\n");
				stringDestination.push_back(static_cast<StringType::value_type>(codePoint));
			}
			else if (codePoint <= 0x7FF)
			{

				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint >> 6) | 0xC0));
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint & 0x3F) | 0x80));
			}
			else if (codePoint <= 0xFFFF)
			{
			    //printf("*********************************");
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint >> 12) | 0xE0));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 6) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint & 0x3F) | 0x80));
				//cout<<"\n"<<stringDestination<<endl;
			}
			else if (codePoint <= 0x1FFFFF)
			{
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint >> 18) | 0xF0));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 12) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 6) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint & 0x3F) | 0x80));
			}
			// Invalid character. Put this anyway.
			else if (codePoint <= 0x3FFFFFF)
			{
			    printf("\nit is invalide\n");
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint >> 24) | 0xF8));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 18) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 12) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 6) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint & 0x3F) | 0x80));
			}
			else
			{
			      printf("\nit is invalide\n");
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint >> 30) | 0xFC));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 24) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 18) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 12) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					((codePoint >> 6) & 0x3F) | 0x80));
				stringDestination.push_back(static_cast<StringType::value_type>(
					(codePoint & 0x3F) | 0x80));
			}
}

void readUTFFile()
{
   printf("\nUTF file is reading..\n");
   in.open ("a.txt",ifstream::in);

   if(in!=NULL)
   {
       printf("\nfile is openned successfully..\n\n");
    }
    else
       printf("\nsome problem in openning the file..\n");

    int result=0;
    string s;
   do {
    int i=ReadCharacter(result);
    if(i==1){
    //printf("\nno of byes read:%c\n",result);
    WriteCharacter(s,result);
    }
  }while (in.good());
  ofstream myfile;
  myfile.open ("b.txt");
  myfile << s;
  cout<<"size  "<<s.size();
  myfile.close();
}

int main()
{
    //string s;
    printf("\nsimple program for encoding..:\n");
    readUTFFile();

    return 0;
}
